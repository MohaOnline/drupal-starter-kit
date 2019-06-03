// For authoring Nightwatch tests, see
// http://nightwatchjs.org/guide#usage

/**
 * Return a CSS selector string to target a specific table cell.
 * @param {string} tBodySelector - CSS selector targeting the body of the table in question.
 * @param {(integer|string)} row - Row number, 1-based.
 * @param {(integer|string)} col - Column number, 1-based.
 * @return {string} CSS selector of the cell.
 */
function cellSelector (tBodySelector, row, col) {
  return tBodySelector + ' > tr:nth-of-type(' + row + ') > td:nth-of-type(' + col + ')'
}

var newCustomDatasetKey

module.exports = {
  'app is being rendered': function (client) {
    var app = client.page.wizardStep()

    app.navigate()

    app.expect.element('@chosenDataset').to.be.present
    app.expect.element('@chosenDataset').value.to.be.equal('mp')

    client.pause(700)

    app.expect.element('@introText').text.to.contain('MPs')
    app.expect.element('@selectOrEditDataset').to.be.visible
    app.expect.section('@selectDialog').not.to.be.visible
    app.expect.section('@editDialog').not.to.be.present
  },

  'choosing a non-custom dataset': function (client) {
    var app = client.page.wizardStep()
    var dialog = app.section.selectDialog
    var list = dialog.section.datasetList

    app.click('@selectOrEditDataset')
    app.waitForElementVisible(dialog.selector, 1000)
    client.assert.elementCount(list.elements.dataset.selector, 3)
    list.setValue('@filter', 'mep')
    client.pause(100)
    client.assert.elementCount(list.elements.dataset.selector, 1)
    list.click('@dataset')
    app.waitForElementNotVisible(dialog.selector, 1000)

    app.expect.element('@chosenDataset').value.to.be.equal('mep')
    app.expect.element('@introText').text.to.contain('MEPs')
  },

  'start adding a custom dataset and cancel': function (client) {
    var app = client.page.wizardStep()
    var selectDialog = app.section.selectDialog
    var editDialog = app.section.editDialog

    app.click('@selectOrEditDataset')
    app.waitForElementVisible(selectDialog.selector, 1000)
    selectDialog.click('@addNewDataset')
    app.waitForElementNotVisible(selectDialog.selector, 1000)
    app.waitForElementVisible(editDialog.selector, 1000)
    editDialog.click('@cancel')
    app.waitForElementNotVisible(editDialog.selector, 1000)

    app.expect.element('@chosenDataset').value.to.be.equal('mep')
    app.expect.element('@introText').text.to.contain('MEPs')
  },

  'add a custom dataset, validate contacts': function (client) {
    var app = client.page.wizardStep()
    var selectDialog = app.section.selectDialog
    var editDialog = app.section.editDialog
    var popup = editDialog.section.popup

    app.click('@selectOrEditDataset')
    app.waitForElementVisible(selectDialog.selector, 1000)
    selectDialog.click('@addNewDataset')
    app.waitForElementNotVisible(selectDialog.selector, 1000)
    app.waitForElementVisible(editDialog.selector, 1000)

    editDialog.setValue('@datasetTitle', 'My first "pretty" dataset')
    editDialog.setValue('@datasetDescription', 'We’re testing.')
    editDialog.click('@addContact')
    client.assert.elementCount(editDialog.elements.tableBody.selector + ' > tr', 1)
    client.assert.elementCount(editDialog.elements.tableBody.selector + ' > tr > td', 7)
    editDialog.expect.element('@deleteContact').to.be.present
    editDialog.expect.element('@invalidContactMark').not.to.be.present

    client.click(cellSelector(editDialog.elements.tableBody.selector, 1, 4)) // first_name
    app.waitForElementVisible(popup.selector, 1000)
    popup.setValue('@input', 'Max')
    popup.click('@save')
    app.waitForElementNotPresent(popup.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 4)).text.to.equal('Max')

    client.click(cellSelector(editDialog.elements.tableBody.selector, 1, 5)) // last_name
    app.waitForElementVisible(popup.selector, 1000)
    popup.setValue('@input', Array(301).join('x')) // Try a string with 300 characters...
    popup.click('@save')
    app.waitForElementVisible(popup.selector + ' .dsa-edit-value-error', 1000) // Should show an error message.
    popup.clearValue('@input').setValue('@input', 'Muster')
    popup.click('@save')
    app.waitForElementNotPresent(popup.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 5)).text.to.equal('Muster')

    client.click(cellSelector(editDialog.elements.tableBody.selector, 1, 6)) // salutation
    app.waitForElementVisible(popup.selector, 1000)
    popup.setValue('@input', 'Mr Max Muster')
    popup.click('@save')
    app.waitForElementNotPresent(popup.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 6)).text.to.equal('Mr Max Muster')

    client
      .listenXHR()
      .pause(500)

    editDialog.click('@save')
    app.waitForElementVisible(editDialog.elements.invalidContactMark.selector, 1000) // email is still missing
    app.expect.section('@editDialog').to.be.visible
    editDialog.expect.element('@filter').value.to.equal('has:error')
    editDialog.clearValue('@filter').setValue('@filter', 'max')
    client.pause(50)

    client.click(cellSelector(editDialog.elements.tableBody.selector, 1, 2)) // email
    app.waitForElementVisible(popup.selector, 1000)
    popup.setValue('@input', 'max.muster@example.com')
    popup.click('@save')
    app.waitForElementNotPresent(popup.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 2)).text.to.equal('max.muster@example.com')

    editDialog.click('@save')
    app.waitForElementNotVisible(editDialog.selector, 1000)
    app.expect.element('@chosenDataset').value.to.match(/^[a-f0-9]{8}-[a-f0-9]{4}-4[a-f0-9]{3}-[89ab][a-f0-9]{3}-[a-f0-9]{12}$/)
    app.expect.element('@introText').text.to.contain('My first "pretty" dataset')

    app.getValue('@chosenDataset', function (result) {
      var datasetKey = newCustomDatasetKey = result.value

      client.getXHR('/', 1000, function (xhrs) {
        client.assert.equal(xhrs.length, 2)
        client.assert.equal(xhrs[0].method, 'POST')
        client.assert.equal(xhrs[0].httpResponseCode, '201')
        client.assert.equal(xhrs[1].method, 'PUT')
        client.assert.equal(xhrs[1].httpResponseCode, '200')

        var dataset = {
          key: datasetKey,
          title: 'My first "pretty" dataset',
          description: 'We’re testing.',
          is_custom: true,
          attributes: [
            {description: '', 'key': 'contact.email', 'title': 'Email address'},
            {description: '', 'key': 'contact.title', 'title': 'Title'},
            {description: '', 'key': 'contact.first_name', 'title': 'First name'},
            {description: '', 'key': 'contact.last_name', 'title': 'Last name'},
            {description: 'Full name and titles', 'key': 'contact.salutation', 'title': 'Salutation'}
          ]
        }
        var contacts = [
          {
            email: 'max.muster@example.com',
            title: '',
            first_name: 'Max',
            last_name: 'Muster',
            salutation: 'Mr Max Muster'
          }
        ]

        client.assert.deepEqual(JSON.parse(xhrs[0].requestData), dataset)
        client.assert.deepEqual(JSON.parse(xhrs[1].requestData), contacts)

        console.log('Asserting mock server response:')
        contacts[0].id = 1
        client.assert.deepEqual(JSON.parse(xhrs[0].responseData), dataset)
        client.assert.deepEqual(JSON.parse(xhrs[1].responseData), contacts)
      })
    })
  },

  'edit custom dataset, discard changes': function (client) {
    var app = client.page.wizardStep()
    var editDialog = app.section.editDialog
    var popup = editDialog.section.popup

    app.click('@selectOrEditDataset')
    app.waitForElementVisible(editDialog.selector, 1000)

    editDialog.expect.element('@datasetTitle').value.to.equal('My first "pretty" dataset')
    editDialog.expect.element('@datasetDescription').value.to.equal('We’re testing.')
    editDialog.expect.element('@alertMessage').not.to.be.present
    client.assert.elementCount(editDialog.elements.tableBody.selector + ' > tr', 1)
    client.assert.elementCount(editDialog.elements.tableBody.selector + ' > tr > td', 7)
    editDialog.expect.element('@deleteContact').to.be.present
    editDialog.expect.element('@invalidContactMark').not.to.be.present
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 2)).text.to.equal('max.muster@example.com')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 3)).text.to.equal('')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 4)).text.to.equal('Max')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 5)).text.to.equal('Muster')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 6)).text.to.equal('Mr Max Muster')

    client.click(cellSelector(editDialog.elements.tableBody.selector, 1, 4)) // first_name
    app.waitForElementVisible(popup.selector, 1000)
    popup.clearValue('@input').setValue('@input', 'Heinrich')
    popup.click('@save')
    app.waitForElementNotPresent(popup.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 4)).text.to.equal('Heinrich')

    editDialog.click('@cancel')

    app.waitForElementVisible(editDialog.elements.alertMessage.selector, 1000)
    app.expect.section('@editDialog').to.be.visible

    editDialog.click('@cancel')

    app.waitForElementNotVisible(editDialog.selector, 1000)
    app.expect.element('@chosenDataset').value.to.equal(newCustomDatasetKey)
    app.expect.element('@introText').text.to.contain('My first "pretty" dataset')
  },

  'choose another custom dataset (see build/api-db.template.json for the fixture)': function (client) {
    var app = client.page.wizardStep()
    var selectDialog = app.section.selectDialog
    var datasetList = selectDialog.section.datasetList
    var editDialog = app.section.editDialog

    app.click('@selectOrEditDataset')
    app.waitForElementVisible(editDialog.selector, 1000)
    editDialog.click('@chooseDataset')
    app.waitForElementNotVisible(editDialog.selector, 1000)
    app.waitForElementVisible(selectDialog.selector, 1000)

    client.assert.elementCount(datasetList.elements.dataset.selector, 4)
    client.click(datasetList.elements.dataset.selector + ':nth-of-type(3)')
    app.waitForElementNotVisible(selectDialog.selector, 1000)
    app.waitForElementVisible(editDialog.selector, 1000)

    editDialog.expect.element('@datasetTitle').value.to.equal('My custum dataset')
    editDialog.expect.element('@datasetDescription').value.to.equal('Lorem ipsum...')
    editDialog.expect.element('@alertMessage').not.to.be.present
    client.assert.elementCount(editDialog.elements.tableBody.selector + ' > tr', 20)
    editDialog.expect.element('@deleteContact').to.be.present
    editDialog.expect.element('@invalidContactMark').not.to.be.present
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 3, 2)).text.to.equal('kweinmann2@webnode.com')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 3, 3)).text.to.equal('Ms')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 3, 4)).text.to.equal('Kenn')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 3, 5)).text.to.equal('Weinmann')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 3, 6)).text.to.equal('Kenn Weinmann')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 3, 7)).text.to.equal('Mauv')
  },

  'edit a contact, delete another one and save the dataset': function (client) {
    var app = client.page.wizardStep()
    var editDialog = app.section.editDialog
    var popup = editDialog.section.popup
    var msgBox = app.section.messageBox

    // change email address and title of Jaclyn Dodworth

    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 4, 2)).text.to.equal('jdodworth3@clickbank.net')
    client.click(cellSelector(editDialog.elements.tableBody.selector, 4, 2))
    app.waitForElementVisible(popup.selector, 1000)
    popup.expect.element('@input').value.to.equal('jdodworth3@clickbank.net')
    popup.clearValue('@input').setValue('@input', 'foobar@baz')
    popup.click('@save')
    app.waitForElementVisible(popup.elements.error.selector, 1000)
    editDialog.expect.section('@popup').to.be.visible
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 4, 2)).text.to.equal('jdodworth3@clickbank.net')

    popup.clearValue('@input').setValue('@input', 'jaclyn.dodworth@government.biz')
    popup.click('@save')
    app.waitForElementNotPresent(popup.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 4, 2)).text.to.equal('jaclyn.dodworth@government.biz')

    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 4, 3)).text.to.equal('Honorable')
    client.click(cellSelector(editDialog.elements.tableBody.selector, 4, 3))
    app.waitForElementVisible(popup.selector, 1000)
    popup.expect.element('@error').not.to.be.present
    popup.expect.element('@input').value.to.equal('Honorable')
    popup.clearValue('@input').setValue('@input', 'Dr')
    popup.click('@save')
    app.waitForElementNotPresent(popup.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 4, 3)).text.to.equal('Dr')

    // accidentially nearly delete Jaclyn Dodworth

    client.click(cellSelector(editDialog.elements.tableBody.selector, 4, 8) + ' ' + editDialog.elements.deleteContact.selector)
    app.waitForElementVisible(msgBox.selector, 1000)
    msgBox.expect.element('@box').to.be.visible
    msgBox.expect.element('@title').to.be.visible
    msgBox.expect.element('@title').text.to.be.equal('Delete contact')
    msgBox.expect.element('@message').to.be.visible
    msgBox.expect.element('@message').text.to.contain('really')
    msgBox.expect.element('@cancel').to.be.visible
    msgBox.expect.element('@ok').to.be.visible
    msgBox.click('@cancel')
    app.waitForElementNotVisible(msgBox.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 4, 6)).text.to.equal('Jaclyn Dodworth')

    // really delete Neddy Grossier (row 2)

    client.click(cellSelector(editDialog.elements.tableBody.selector, 2, 8) + ' ' + editDialog.elements.deleteContact.selector)
    app.waitForElementVisible(msgBox.selector, 1000)
    msgBox.expect.element('@box').to.be.visible
    msgBox.expect.element('@cancel').to.be.visible
    msgBox.expect.element('@ok').to.be.visible
    msgBox.click('@ok')
    app.waitForElementNotVisible(msgBox.selector, 1000)
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 1, 6)).text.to.equal('Lenette Barnby')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 2, 6)).text.to.equal('Kenn Weinmann')
    app.expect.element(cellSelector(editDialog.elements.tableBody.selector, 3, 6)).text.to.equal('Jaclyn Dodworth')

    editDialog.clearValue('@datasetTitle').setValue('@datasetTitle', 'My really fancy dataset')

    client
      .listenXHR()
      .pause(500)

    editDialog.click('@save')

    app.waitForElementNotVisible(editDialog.selector, 1000)
    app.expect.element('@chosenDataset').value.to.equal('39f93565-d491-4e3f-99a5-fc5c83446e2f')
    app.expect.element('@introText').text.to.contain('My really fancy dataset')

    client.getXHR('/', 1000, function (xhrs) {
      client.assert.equal(xhrs.length, 2)
      client.assert.equal(xhrs[0].method, 'PUT')
      client.assert.equal(xhrs[0].httpResponseCode, '200')
      client.assert.equal(xhrs[1].method, 'PUT')
      client.assert.equal(xhrs[1].httpResponseCode, '200')

      var dataset = JSON.parse(xhrs[0].requestData)
      var contacts = JSON.parse(xhrs[1].requestData)

      client.assert.equal(dataset.key, '39f93565-d491-4e3f-99a5-fc5c83446e2f')
      client.assert.equal(dataset.title, 'My really fancy dataset')
      client.assert.equal(dataset.description, 'Lorem ipsum...')
      client.assert.equal(dataset.is_custom, true)
      client.assert.equal(dataset.attributes.length, 6)
      client.assert.equal(contacts.length, 24)
      client.assert.equal(contacts[0].id, 1)
      client.assert.equal(contacts[0].salutation, 'Lenette Barnby')
      client.assert.equal(contacts[1].id, 3)
      client.assert.equal(contacts[1].salutation, 'Kenn Weinmann')
      client.assert.equal(contacts[2].id, 4)
      client.assert.equal(contacts[2].salutation, 'Jaclyn Dodworth')
    })

    client.end()
  }
}
