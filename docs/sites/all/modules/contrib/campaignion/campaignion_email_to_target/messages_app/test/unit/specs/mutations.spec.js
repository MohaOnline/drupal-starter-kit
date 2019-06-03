import mutations from '@/store/mutations'
import initialState from '@/store/state'
import testData from '../fixtures/example-data'
import find from 'lodash.find'

describe('mutations', function () {
  var state

  beforeEach(function () {
    state = Object.assign({}, initialState)
  })

  describe('initializeData', function () {
    it('parses bootstrapped data', function () {
      var compareSpec = testData.messageSelection[0]
      compareSpec.filters[0].attributeLabel = 'Political Affiliation'

      mutations.initializeData(state, testData)

      expect(state.defaultMessage.message).to.deep.equal(testData.messageSelection[testData.messageSelection.length - 1].message)
      expect(state.specs.length).to.equal(testData.messageSelection.length - 1)
      expect(state.specs[0]).to.deep.equal(compareSpec)
      expect(state.targetAttributes).to.deep.equal(testData.targetAttributes)
      expect(state.tokenCategories).to.deep.equal(testData.tokens)
      expect(state.hardValidation).to.equal(testData.hardValidation)
      expect(state.specs[0].filters[0].attributeLabel).to.equal(compareSpec.filters[0].attributeLabel)
    })
  })

  describe('validateSpecs', function () {
    beforeEach(() => {
      mutations.initializeData(state, testData)
      mutations.validateSpecs(state)
    })

    it('checks if a spec lacks a filter', function () {
      var s = find(state.specs, ['label', 'exclusion without a filter'])
      expect(s.errors).to.deep.include({type: 'filter', message: 'No filter selected'})
    })

    it('checks if a filter lacks a value', function () {
      var s = find(state.specs, ['label', 'message with a previously used filter and a missing filter value'])
      expect(s.errors).to.deep.include({type: 'filter', message: 'A filter value is missing'})
    })

    it('checks if a message is empty or consists of white space only', function () {
      var s = find(state.specs, ['label', 'same filter as message above, empty message'])
      expect(s.errors).to.deep.include({type: 'message', message: 'Message is empty'})
    })

    it('checks if a combination of the same filters has been used by a preceding spec', function () {
      var s1 = find(state.specs, ['label', 'shares a filter with first message'])
      var s2 = find(state.specs, ['label', 'shares both filters with preceding message'])
      expect(s1.errors).not.to.deep.include({type: 'filter', message: 'This message won’t be sent. The same filter has been applied above.'})
      expect(s2.errors).to.deep.include({type: 'filter', message: 'This exclusion won’t be taken into account. The same filter has been applied above.'})
    })

    it('ignores specs that share only some filters', function () {
      var s1 = find(state.specs, ['label', 'shares a filter with first message'])
      var s2 = find(state.specs, ['label', 'same filter as message above, empty message'])
      expect(s1.errors).not.to.deep.include({type: 'filter', message: 'This message won’t be sent. The same filter has been applied above.'})
      expect(s2.errors).not.to.deep.include({type: 'filter', message: 'This message won’t be sent. The same filter has been applied above.'})
    })

    it('ignores the filter order if a spec has other filter errors', function () {
      var s = find(state.specs, ['label', 'message with a previously used filter and a missing filter value'])
      expect(s.errors).not.to.deep.include({type: 'filter', message: 'This message won’t be sent. The same filter has been applied above.'})
    })
  })
})
