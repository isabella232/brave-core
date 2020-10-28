/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

import modalBackground from './assets/opt_in_modal_bg.svg'

export const root = styled.div`
  position: fixed;
  top: 0;
  left: 0;
  bottom: 0;
  right: 0;
  background: rgba(0, 0, 0, 0.33);
  z-index: 9999;
  display: flex;
  justify-content: space-around;
  align-items: center;
  padding: 0 20px 50px;
`

export const content = styled.div`
  font-family: var(--brave-font-heading);
  text-align: center;
  max-width: 335px;
  background-color: var(--brave-palette-white);
  background-image: url(${modalBackground});
  background-repeat: no-repeat;
  background-position: 4px -11px;
  background-size: auto 200px;
  box-shadow: 0px 0px 16px rgba(99, 105, 110, 0.2);
  border-radius: 8px;
  padding: 17px;
`

export const close = styled.div`
  color: var(--brave-palette-neutral600);
  text-align: right;

  button {
    margin: 0;
    padding: 2px;
    background: none;
    border: none;
    cursor: pointer;
  }

  .icon {
    display: block;
    width: 14px;
    height: auto;
  }
`

export const header = styled.div`
  margin-top: 17px;
  color: var(--brave-palette-black);
  font-weight: 600;
  font-size: 18px;
  line-height: 22px;
`

export const batIcon = styled.div`
  display: inline-block;
  vertical-align: middle;
  width: 26px;
  margin-right: 7px;
`

export const text = styled.div`
  margin: 8px 6px 0;
  color: var(--brave-palette-neutral700);
  font-size: 14px;
  line-height: 24px;
`

export const enable = styled.div`
  margin-top: 60px;
`

export const addFunds = styled.div`
  margin-top: 18px;
  color: var(--brave-color-brandBat);

  button {
    font-weight: 600;
    font-size: 14px;
    line-height: 21px;
    border: 0;
    background: 0;
    margin: 0;
    padding: 0;
    cursor: pointer;
  }

  svg {
    width: 16px;
    height: auto;
    vertical-align: middle;
    margin-right: 5px;
    margin-bottom: 2px;
  }
`

export const terms = styled.div`
  margin: 32px 14px 15px;
  color: var(--brave-palette-neutral600);
  font-size: 11px;
  line-height: 16px;

  a {
    color: var(--brave-color-brandBat);
    font-weight: 600;
    text-decoration: none;
  }
`

export const addFundsItem = styled.div`
  margin-top: 24px;
  color: var(--brave-palette-neutral900);
  font-weight: 600;
  font-size: 14px;
  line-height: 20px;

  .icon {
    color: var(--brave-palette-neutral600);
    width: 24px;
    vertical-align: middle;
    margin-right: 16px;
    margin-bottom: 3px;
  }
`

export const addFundsAction = styled.div`
  margin-top: 31px;
`
