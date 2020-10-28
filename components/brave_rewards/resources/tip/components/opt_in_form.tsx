/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { EmoteSadIcon } from 'brave-ui/components/icons'

import { TipOptInForm } from '../../shared/onboarding'

import * as style from './opt_in_form.style'

interface Props {
  onEnable: () => void
  onDismiss: () => void
}

export function OptInForm (props: Props) {
  return (
    <style.root>
      <style.topBar>
        <style.sadIcon><EmoteSadIcon /></style.sadIcon>
        Hold on, you can’t tip yet
      </style.topBar>
      <style.content>
        <TipOptInForm onEnable={props.onEnable} onDismiss={props.onDismiss} />
      </style.content>
    </style.root>
  )
}
