import Vue from 'vue'
import {clone} from '@/utils'

export default {
  /**
   * Initialize the store.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {Object[]} payload.redirects - Collection of redirects, Collection of objects describing the target attributes: {name: 'contact.email', label: 'Email address', description: ''}including the default redirect as the last item.
   * @param {string} payload.defaultRedirectUrl - If there is no redirects array or if it’s empty, use this url for the default redirect.
   */
  initData (state, {redirects, defaultRedirectUrl}) {
    if (defaultRedirectUrl && (typeof redirects === 'undefined' || !redirects.length)) {
      state.defaultRedirect.destination = defaultRedirectUrl
      state.defaultRedirect.prettyDestination = defaultRedirectUrl
    } else {
      state.defaultRedirect = clone(redirects[redirects.length - 1])
      state.redirects = clone(redirects).slice(0, -1)
    }

    // Preserve initial state
    state.initialData.redirects = clone(state.redirects)
    state.initialData.defaultRedirect = clone(state.defaultRedirect)
  },

  /**
   * Start editing a new redirect.
   * @param {Object} state - vuex state.
   */
  editNewRedirect (state) {
    state.currentRedirectIndex = -1
  },

  /**
   * Start editing an existing redirect.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {integer} payload.index - The redirect’s index in the redirect array.
   */
  editRedirect (state, {index}) {
    state.currentRedirectIndex = index
  },

  /**
   * Delete a redirect from the redirects array.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {integer} payload.index - The redirect’s index in the redirects array.
   */
  removeRedirect (state, {index}) {
    state.redirects.splice(index, 1)
  },

  /**
   * Stop editing a redirect and leave it alone.
   * @param {Object} state - vuex state.
   */
  leaveRedirect (state) {
    state.currentRedirectIndex = null
  },

  /**
   * Save a redirect to the store.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {Object} payload.redirect - The redirect to be saved.
   */
  updateRedirect (state, {redirect}) {
    if (state.currentRedirectIndex === null) return
    if (state.currentRedirectIndex === -1) {
      state.redirects.push(redirect)
    } else {
      Vue.set(state.redirects, state.currentRedirectIndex, clone(redirect))
    }
  },

  /**
   * Save the default redirect to the store.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {Object} payload.destination - The default redirect’s internal value.
   * @param {Object} payload.prettyDestination - The default redirect’s display value.
   */
  updateDefaultRedirect (state, {destination, prettyDestination}) {
    state.defaultRedirect.destination = destination
    state.defaultRedirect.prettyDestination = prettyDestination
  },

  /**
   * Replace all redirects in the store.
   * @param {Object} state - vuex state.
   * @param {Object} payload - The mutation’s payload.
   * @param {Object[]} payload.redirects - The collection of redirects to be saved.
   */
  updateRedirects (state, {redirects}) {
    state.redirects = redirects
  }
}
