/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ad_serving/ad_notifications/ad_notification_serving.h"

#include <stdint.h>

#include <string>
#include <vector>

#include "base/guid.h"
#include "base/rand_util.h"
#include "bat/ads/ad_notification_info.h"
#include "bat/ads/internal/ad_delivery/ad_notifications/ad_notification_delivery.h"
#include "bat/ads/internal/ad_pacing/ad_notifications/ad_notification_pacing.h"
#include "bat/ads/internal/ad_targeting/ad_targeting_util.h"
#include "bat/ads/internal/ads_impl.h"
#include "bat/ads/internal/client/client.h"
#include "bat/ads/internal/database/tables/ad_events_database_table.h"
#include "bat/ads/internal/database/tables/creative_ad_notifications_database_table.h"
#include "bat/ads/internal/eligible_ads/ad_notifications/eligible_ad_notifications.h"
#include "bat/ads/internal/frequency_capping/ad_notifications/ad_notifications_frequency_capping.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/p2a/p2a.h"
#include "bat/ads/internal/p2a/p2a_util.h"
#include "bat/ads/internal/platform/platform_helper.h"
#include "bat/ads/pref_names.h"

namespace ads {
namespace ad_notifications {

AdServing::AdServing(
    AdsImpl* ads)
    : ads_(ads) {
  DCHECK(ads_);
}

AdServing::~AdServing() = default;

void AdServing::ServeAtRegularIntervals() {
  if (timer_.IsRunning()) {
    return;
  }

  base::TimeDelta delay;

  if (ads_->get_client()->GetNextAdServingInterval().is_null()) {
    delay = base::TimeDelta::FromMinutes(2);
    const base::Time next_interval = base::Time::Now() + delay;

    ads_->get_client()->SetNextAdServingInterval(next_interval);
  } else {
    if (NextIntervalHasElapsed()) {
      delay = base::TimeDelta::FromMinutes(1);
    } else {
      const base::Time next_interval =
          ads_->get_client()->GetNextAdServingInterval();

      delay = next_interval - base::Time::Now();
    }
  }

  const base::Time next_interval = MaybeServeAfter(delay);

  BLOG(1, "Maybe serve ad notification " << FriendlyDateAndTime(next_interval));
}

void AdServing::StopServing() {
  timer_.Stop();
}

void AdServing::MaybeServe() {
  const CategoryList categories =
      ads_->get_ad_targeting()->GetWinningCategories();

  MaybeServeAdForCategories(categories, [&](
      const Result result,
      const AdNotificationInfo& ad) {
    if (result != Result::SUCCESS) {
      BLOG(1, "Ad notification not delivered");
      FailedToDeliverAd();
      return;
    }

    BLOG(1, "Ad notification delivered:\n"
        << "  uuid: " << ad.uuid << "\n"
        << "  creativeInstanceId: " << ad.creative_instance_id << "\n"
        << "  creativeSetId: " << ad.creative_set_id << "\n"
        << "  campaignId: " << ad.campaign_id << "\n"
        << "  category: " << ad.category << "\n"
        << "  title: " << ad.title << "\n"
        << "  body: " << ad.body << "\n"
        << "  targetUrl: " << ad.target_url);

    DeliveredAd();
  });
}

///////////////////////////////////////////////////////////////////////////////

bool AdServing::NextIntervalHasElapsed() {
  const base::Time now = base::Time::Now();

  const base::Time next_interval =
      ads_->get_client()->GetNextAdServingInterval();

  if (now < next_interval) {
    return false;
  }

  return true;
}

base::Time AdServing::MaybeServeAfter(
    const base::TimeDelta delay) {
  return timer_.Start(delay, base::BindOnce(&AdServing::MaybeServe,
      base::Unretained(this)));
}

void AdServing::MaybeServeAdForCategories(
    const CategoryList& categories,
    MaybeServeAdForCategoriesCallback callback) {
  database::table::AdEvents database_table(ads_);
  database_table.GetAll([=](
      const Result result,
      const AdEventList& ad_events) {
    if (result != Result::SUCCESS) {
      BLOG(1, "Ad notification not served: Failed to get ad events");
      callback(Result::FAILED, AdNotificationInfo());
      return;
    }

    FrequencyCapping frequency_capping(ads_, ad_events);

    if (!frequency_capping.IsAdAllowed()) {
      BLOG(1, "Ad notification not served: Not allowed");
      callback(Result::FAILED, AdNotificationInfo());
      return;
    }

    RecordAdOpportunityForCategories(categories);

    MaybeServeAdForParentChildCategories(categories, ad_events, callback);
  });
}

void AdServing::MaybeServeAdForParentChildCategories(
    const CategoryList& categories,
    const AdEventList& ad_events,
    MaybeServeAdForCategoriesCallback callback) {
  if (categories.empty()) {
    BLOG(1, "No categories to serve targeted ads");
    MaybeServeAdForUntargeted(ad_events, callback);
    return;
  }

  BLOG(1, "Serve ad for categories:");
  for (const auto& category : categories) {
    BLOG(1, "  " << category);
  }

  database::table::CreativeAdNotifications database_table(ads_);
  database_table.GetForCategories(categories, [=](
      const Result result,
      const CategoryList& categories,
      const CreativeAdNotificationList& ads) {
    EligibleAds eligible_ad_notifications(ads_);

    const CreativeAdNotificationList eligible_ads =
        eligible_ad_notifications.Get(ads,
            last_delivered_creative_ad_, ad_events);

    if (eligible_ads.empty()) {
      BLOG(1, "No eligible ads found for categories");
      MaybeServeAdForParentCategories(categories, ad_events, callback);
      return;
    }

    MaybeServeAd(eligible_ads, callback);
  });
}

void AdServing::MaybeServeAdForParentCategories(
    const CategoryList& categories,
    const AdEventList& ad_events,
    MaybeServeAdForCategoriesCallback callback) {
  const CategoryList parent_categories =
      ad_targeting::GetParentCategories(categories);

  BLOG(1, "Serve ad for parent categories:");
  for (const auto& parent_category : parent_categories) {
    BLOG(1, "  " << parent_category);
  }

  database::table::CreativeAdNotifications database_table(ads_);
  database_table.GetForCategories(parent_categories, [=](
      const Result result,
      const CategoryList& categories,
      const CreativeAdNotificationList& ads) {
    EligibleAds eligible_ad_notifications(ads_);

    const CreativeAdNotificationList eligible_ads =
        eligible_ad_notifications.Get(ads,
            last_delivered_creative_ad_, ad_events);

    if (eligible_ads.empty()) {
      BLOG(1, "No eligible ads found for parent categories");
      MaybeServeAdForUntargeted(ad_events, callback);
      return;
    }

    MaybeServeAd(eligible_ads, callback);
  });
}

void AdServing::MaybeServeAdForUntargeted(
    const AdEventList& ad_events,
    MaybeServeAdForCategoriesCallback callback) {
  BLOG(1, "Serve untargeted ad");

  const std::vector<std::string> categories = {
    ad_targeting::contextual::kUntargeted
  };

  database::table::CreativeAdNotifications database_table(ads_);
  database_table.GetForCategories(categories, [=](
      const Result result,
      const CategoryList& categories,
      const CreativeAdNotificationList& ads) {
    EligibleAds eligible_ad_notifications(ads_);

    const CreativeAdNotificationList eligible_ads =
        eligible_ad_notifications.Get(ads,
            last_delivered_creative_ad_, ad_events);

    if (eligible_ads.empty()) {
      BLOG(1, "No eligible ads found for untargeted category");
      BLOG(1, "Ad notification not served: No eligible ads found");
      callback(Result::FAILED, AdNotificationInfo());
      return;
    }

    MaybeServeAd(eligible_ads, callback);
  });
}

void AdServing::MaybeServeAd(
    const CreativeAdNotificationList& ads,
    MaybeServeAdForCategoriesCallback callback) {
  CreativeAdNotificationList eligible_ads = PaceAds(ads);
  if (eligible_ads.empty()) {
    BLOG(1, "Ad notification not served: No eligible ads found");
    callback(Result::FAILED, AdNotificationInfo());
    return;
  }

  BLOG(1, "Found " << eligible_ads.size() << " eligible ads");

  const int rand = base::RandInt(0, eligible_ads.size() - 1);
  const CreativeAdNotificationInfo ad = eligible_ads.at(rand);
  MaybeDeliverAd(ad, callback);
}

CreativeAdNotificationList AdServing::PaceAds(
    const CreativeAdNotificationList& ads) {
  BLOG(2, ads.size() << " eligible ads before pacing");

  AdPacing ad_pacing;
  CreativeAdNotificationList paced_ads = ad_pacing.PaceAds(ads);

  BLOG(2, paced_ads.size() << " eligible ads after pacing");

  return paced_ads;
}

void AdServing::MaybeDeliverAd(
    const CreativeAdNotificationInfo& ad,
    MaybeServeAdForCategoriesCallback callback) {
  AdNotificationInfo ad_notification;
  ad_notification.type = AdType::kAdNotification;
  ad_notification.uuid = base::GenerateGUID();
  ad_notification.creative_instance_id = ad.creative_instance_id;
  ad_notification.creative_set_id = ad.creative_set_id;
  ad_notification.campaign_id = ad.campaign_id;
  ad_notification.category = ad.category;
  ad_notification.title = ad.title;
  ad_notification.body = ad.body;
  ad_notification.target_url = ad.target_url;

  AdDelivery ad_delivery(ads_);
  if (!ad_delivery.MaybeDeliverAd(ad_notification)) {
    BLOG(1, "Ad notification not delivered");
    callback(Result::FAILED, ad_notification);
    return;
  }

  last_delivered_creative_ad_ = ad;

  ads_->get_client()->UpdateSeenAdvertiser(ad.advertiser_id);

  callback(Result::SUCCESS, ad_notification);
}

void AdServing::FailedToDeliverAd() {
  if (!PlatformHelper::GetInstance()->IsMobile()) {
    return;
  }

  const base::TimeDelta delay = base::TimeDelta::FromMinutes(2);

  const base::Time next_interval = base::Time::Now() + delay;
  ads_->get_client()->SetNextAdServingInterval(next_interval);

  MaybeServeAfter(delay);
}

void AdServing::DeliveredAd() {
  if (!PlatformHelper::GetInstance()->IsMobile()) {
    return;
  }

  const int64_t seconds = base::Time::kSecondsPerHour /
      ads_->get_ads_client()->GetUint64Pref(prefs::kAdsPerHour);

  const base::TimeDelta delay = base::TimeDelta::FromSeconds(seconds);

  const base::Time next_interval = base::Time::Now() + delay;

  ads_->get_client()->SetNextAdServingInterval(next_interval);

  MaybeServeAfter(delay);
}

void AdServing::RecordAdOpportunityForCategories(
    const CategoryList& categories) {
  const std::vector<std::string> question_list =
      CreateAdOpportunityQuestionList(categories);

  P2A p2a(ads_);
  p2a.RecordEvent("ad_opportunity", question_list);
}

}  // namespace ad_notifications
}  // namespace ads
