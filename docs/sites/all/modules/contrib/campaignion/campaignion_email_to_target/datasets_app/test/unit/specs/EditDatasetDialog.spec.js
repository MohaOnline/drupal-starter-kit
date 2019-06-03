import assert from 'assert'

import EditDatasetDialog from '@/components/EditDatasetDialog'

describe('EditDatasetDialog', function () {
  describe('computed', function () {
    describe('contentColumns', function () {
      const contentColumns = EditDatasetDialog.computed.contentColumns.bind({
        tableColumns: ['__error', 'a__strange_field_name', 'first_name', 'last_name', '__foo', 'bar__', '__delete']
      })
      it('returns an array of all column keys not starting with a double underscore.', function () {
        assert.deepEqual(contentColumns(), ['a__strange_field_name', 'first_name', 'last_name', 'bar__'])
      })
    })
  })

  describe('methods', function () {
    describe('isValidValue', function () {
      const isValidValue = EditDatasetDialog.methods.isValidValue.bind({
        validations: {first_name: 'Eva', last_name: '\\S+'},
        maxFieldLengths: {last_name: 3, email: 5}
      })
      it('returns true if the regex matches.', function () {
        assert.equal(isValidValue('first_name', 'Eva...'), true)
      })
      it('returns true if the length is fine.', function () {
        assert.equal(isValidValue('email', '12345'), true)
      })
      it('returns true if the regex matches and the length is fine.', function () {
        assert.equal(isValidValue('last_name', 'Lee'), true)
      })
      it('returns false if the regex test fails.', function () {
        assert.equal(isValidValue('first_name', 'Heidi'), false)
      })
      it('returns false if the value is too long.', function () {
        assert.equal(isValidValue('email', 'heidi@hotmail.com'), false)
      })
      it('returns true if there is no test for this column.', function () {
        assert.equal(isValidValue('title', 'Sir'), true)
      })
    })

    describe('serializeContacts', function () {
      const serializeContacts = EditDatasetDialog.methods.serializeContacts
      const contacts = [
        {a: '1', b: '2', foo: 'bar', c: '3'},
        {a: 'A', b: 'B', foo: 'baz', c: 'C'},
        {a: 'a', b: 'b', foo: 'bim', c: 'c'}
      ]
      const columns = [
        {key: 'a', title: 'aaa'},
        {key: 'b', title: 'bbb'},
        {key: 'c', titel: 'ccc'}
      ]
      const csv = 'a,b,c\r\n1,2,3\r\nA,B,C\r\na,b,c'
      it('returns CSV, omitting the foo column.', function () {
        assert.equal(serializeContacts.call({contacts, columns}), csv)
      })
      it('returns comma-separated headers if the contacts are empty.', function () {
        assert.equal(serializeContacts.call({contacts: [], columns}), 'a,b,c')
      })
    })

    describe('generateFilename', function () {
      const generateFilename = EditDatasetDialog.methods.generateFilename
      it('turns special characters into single dashes.', function () {
        assert.equal(generateFilename('a,b.;/c?!d:@=e&"\'<>f#%g{}|\\^~['), 'a-b-c-d-e-f-g.csv')
      })
      it('turns whitespace into single dashes.', function () {
        assert.equal(generateFilename('a    b c\nd\r\n e'), 'a-b-c-d-e.csv')
      })
      it('trims leading and trailing whitespace or special characters.', function () {
        assert.equal(generateFilename(' ;#"abc ?  '), 'abc.csv')
      })
      it('encodes utf characters.', function () {
        assert.equal(generateFilename('✓'), '%E2%9C%93.csv')
      })
      it('returns dataset.csv as a default.', function () {
        assert.equal(generateFilename(''), 'dataset.csv')
      })
    })

    describe('columnHeaderTooltipText', function () {
      const columnHeaderTooltipText = EditDatasetDialog.methods.columnHeaderTooltipText
      const column = {key: 'b', title: 'bbb', description: 'old description belonging to the dataset'}
      const customColumn = {key: 'custom', title: 'Custom', description: 'My custom column'}
      const standardColumns = [
        {key: 'a', title: 'aaa', description: 'bar'},
        {key: 'b', title: 'bbb', description: 'updated description set by the server'},
        {key: 'c', title: 'ccc', description: 'foo'}
      ]
      it('returns the description from `this.standardColumns`.', function () {
        assert(columnHeaderTooltipText.call({standardColumns}, column) === 'updated description set by the server')
      })
      it('returns the column’s description if this column is not found in `this.standardColumns`.', function () {
        assert(columnHeaderTooltipText.call({standardColumns}, customColumn) === 'My custom column')
      })
    })
  })
})
