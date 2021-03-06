/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// clang-format off
// #import {addSingletonGetter, sendWithPromise} from 'chrome://resources/js/cr.m.js';
// clang-format on

cr.define('settings', function() {
  /** @interface */
  /* #export */ class BraveNewTabBrowserProxy {
    /**
     * @return {!Promise<Boolean>}
     */
    getIsSuperReferralActive() {}

    /**
     * @return {!Promise<Boolean>}
     */
    getIsBinanceSupported() {}

    /**
     * @return {!Promise<Boolean>}
     */
    getIsBraveTogetherSupported() {}

    /**
     * @return {!Promise<Boolean>}
     */
    getIsGeminiSupported() {}

    /**
     * @return {!Promise<Boolean>}
     */
    getIsBitcoinDotComSupported() {}

    /**
     * @return {!Promise<Boolean>}
     */
    getShowTopSites() {}

    /**
     */
    toggleTopSitesVisible() {}

    /**
     * @return {!Promise<Boolean>}
     */
    isCryptoDotComSupported() {}

    /**
     * @return {!Promise<Array>}
     */
    getNewTabShowsOptionsList() {}

    /**
     * @return {!Promise<Boolean>}
     */
    shouldShowNewTabDashboardSettings() {}
  }

  /**
   * @implements {settings.BraveNewTabBrowserProxy}
   */
  /* #export */ class BraveNewTabBrowserProxyImpl {
    /** @override */
    getIsSuperReferralActive() {
      return cr.sendWithPromise('getIsSuperReferralActive');
    }

    /** @override */
    getIsBinanceSupported() {
      return cr.sendWithPromise('getIsBinanceSupported')
    }

    /** @override */
    getIsBraveTogetherSupported() {
      return cr.sendWithPromise('getIsBraveTogetherSupported')
    }

    /** @override */
    getIsGeminiSupported() {
      return cr.sendWithPromise('getIsGeminiSupported')
    }

    /** @override */
    getIsBitcoinDotComSupported() {
      return cr.sendWithPromise('getIsBitcoinDotComSupported')
    }

    /** @override */
    getShowTopSites() {
      return cr.sendWithPromise('getShowTopSites')
    }

    /** @override */
    toggleTopSitesVisible() {
      chrome.send('toggleTopSitesVisible', [])
    }

    /** @override */
    getIsCryptoDotComSupported() {
      return cr.sendWithPromise('getIsCryptoDotComSupported')
    }

    /** @override */
    getNewTabShowsOptionsList() {
      return cr.sendWithPromise('getNewTabShowsOptionsList')
    }

    /** @override */
    shouldShowNewTabDashboardSettings() {
      return cr.sendWithPromise('shouldShowNewTabDashboardSettings')
    }
  }

  cr.addSingletonGetter(BraveNewTabBrowserProxyImpl);

  // #cr_define_end
  return {
    BraveNewTabBrowserProxy,
    BraveNewTabBrowserProxyImpl
  };
});
