/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { BatColorIcon, WalletAddIcon } from 'brave-ui/components/icons'

import { LocaleContext } from '../lib/locale_context'
import { CloseIcon } from '../components/icons/close_icon'
import { TermsOfService } from '../components/terms_of_service'
import { MainButton } from './main_button'
import { IdIcon } from './icons/id_icon'
import { CameraIcon } from './icons/camera_icon'

import * as style from './rewards_opt_in_modal.style'

interface Props {
  onClose: () => void
  onEnable: () => void
  onAddFunds: () => void
}

export function RewardsOptInModal (props: Props) {
  const { getString } = React.useContext(LocaleContext)
  const [showAddFunds, setShowAddFunds] = React.useState(false)

  const onClickAddFunds = () => setShowAddFunds(true)

  const renderOptIn = () => (
    <>
      <style.header>
        <style.batIcon><BatColorIcon /></style.batIcon>
        {getString('earnTokensHeader')}
      </style.header>
      <style.text>
        {getString('earnTokensText')}
      </style.text>
      <style.enable>
        <MainButton onClick={props.onEnable}>
          {getString('startUsingRewards')}
        </MainButton>
      </style.enable>
      <style.addFunds>
        <button onClick={onClickAddFunds}>
          <WalletAddIcon />{getString('addFunds')}
        </button>
      </style.addFunds>
    </>
  )

  const renderAddFunds = () => (
    <>
      <style.header>
        {getString('addFundsHeader')}
      </style.header>
      <style.text>
        {getString('addFundsText')}
      </style.text>
      <style.addFundsItem>
        <IdIcon />{getString('addFundsId')}
      </style.addFundsItem>
      <style.addFundsItem>
        <CameraIcon />{getString('addFundsPhoto')}
      </style.addFundsItem>
      <style.addFundsAction>
        <MainButton onClick={props.onAddFunds}>
          {getString('takeMeToUphold')}
        </MainButton>
      </style.addFundsAction>
    </>
  )

  return (
    <style.root>
      <style.content>
        <style.close>
          <button onClick={props.onClose}><CloseIcon /></button>
        </style.close>
        {showAddFunds ? renderAddFunds() : renderOptIn()}
        <style.terms>
          <TermsOfService />
        </style.terms>
      </style.content>
    </style.root>
  )
}
