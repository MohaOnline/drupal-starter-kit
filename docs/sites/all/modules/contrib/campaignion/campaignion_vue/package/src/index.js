import 'es6-promise/dist/es6-promise.auto.js'

// Add these modules to 'externals' in your app’s webpack.prod.conf.js
import Vue from 'vue'
import Vuex from 'vuex'
import axios from 'axios'
import {
  Button,
  Dialog,
  Dropdown,
  DropdownItem,
  DropdownMenu,
  MessageBox,
  Loading,
  Option,
  Radio,
  RadioGroup,
  Select
} from 'element-ui'
import DestinationField from './components/DestinationField.vue'
import elementLocale from 'element-ui/lib/locale'
import draggable from 'vuedraggable'
import utils from './utils'

const campaignionVue = {
  Vue,
  Vuex,
  axios,
  element: {
    Button,
    Dialog,
    Dropdown,
    DropdownItem,
    DropdownMenu,
    MessageBox,
    Loading,
    Option,
    Radio,
    RadioGroup,
    Select
  },
  DestinationField,
  elementLocale,
  draggable,
  utils
}

export default campaignionVue
