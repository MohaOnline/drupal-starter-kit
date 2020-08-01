import axios from 'axios'

const url = Drupal.settings.campaignion_email_to_target.endpoints['e2t-api'].url + '/'
const headers = {
  Authorization: 'Bearer ' + Drupal.settings.campaignion_email_to_target.endpoints['e2t-api'].token
}

export default {
  /**
   * Get the available datasets.
   * @return {Promise} axios response.
   */
  getDatasets () {
    return axios.get(url, {headers})
  },

  /**
   * Get the list of contacts for a given dataset.
   * @param {string} datasetKey - Unique identifier for the dataset.
   * @return {Promise} axios response.
   */
  getContacts (datasetKey) {
    return axios.get(url + datasetKey + '/contact', {headers})
  },

  /**
   * Persist a dataset: Create a new one or update an existing one.
   * @param {Object} dataset - The dataset to persist.
   * @param {boolean} createNew - Flag indicating that a new dataset should be created.
   * @return {Promise} axios response.
   */
  saveDataset (dataset, createNew) {
    return axios({
      method: createNew ? 'post' : 'put',
      url: createNew ? url : url + dataset.key,
      data: JSON.stringify(dataset),
      headers,
      transformRequest: [function (data, headers) {
        headers.post = {'Content-Type': 'application/json'}
        headers.put = {'Content-Type': 'application/json'}
        return data
      }]
    })
  },

  /**
   * Persist a list of contacts with a given dataset, replacing the existing list.
   * @param {string} datasetKey - Unique identifier of the dataset.
   * @param {Object[]} contacts - The array of contact objects.
   * @return {Promise} axios response.
   */
  saveContacts (datasetKey, contacts) {
    return axios({
      method: 'put',
      url: url + datasetKey + '/contact',
      data: JSON.stringify(contacts),
      headers,
      transformRequest: [function (data, headers) {
        headers.post = {'Content-Type': 'application/json'}
        headers.put = {'Content-Type': 'application/json'}
        return data
      }]
    })
  }
}
