import {emptyRedirect} from '@/utils/defaults'

/**
 * State factory function.
 * @return {Object} The initial vuex store state.
 * {Object[]} createState().redirects - Collection of custom redirects.
 * {Object} createState().defaultRedirect - The default redirect.
 * {(integer|null)} createState().currentRedirectIndex - The index of the item in
 *    the redirects array that is currently being edited or `-1` for a new item.
 *    `null` means that no item is being edited.
 * {Object} createState().initialData - The `defaultRedirect` and `redirects`
 *    that were initially loaded.
 */
export function createState () {
  return {
    redirects: [],
    defaultRedirect: emptyRedirect(),
    currentRedirectIndex: null,
    initialData: {
      redirects: [],
      defaultRedirect: {}
    }
  }
}
