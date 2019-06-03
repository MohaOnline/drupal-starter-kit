import Vue from 'vue'
import Vuex from 'vuex'
import {createState} from './state'
import actions from './actions'
import mutations from './mutations'

Vue.use(Vuex)

const debug = process.env.NODE_ENV !== 'production'

/**
 * Store factory function.
 * @return {Store} A new instance of Vuex.Store.
 */
export function createStore () {
  return new Vuex.Store({
    state: createState(),
    actions,
    mutations,
    strict: debug
  })
}
