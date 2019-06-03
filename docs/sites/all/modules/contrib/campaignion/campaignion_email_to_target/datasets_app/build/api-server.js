if (process.env.E2T_API_TOKEN) {
  console.log('Using real API. No need for JSON Server.')
  process.exit()
}

const fs = require('fs')
const path = require('path')

// Copy database file from template
fs.writeFileSync(path.join(__dirname, '/api-db.json'), fs.readFileSync(path.join(__dirname, '/api-db.template.json')))

const jsonServer = require('json-server')
const rewrite = require('express-urlrewrite')
const _ = require('lodash')
const server = jsonServer.create()
const router = jsonServer.router(path.join(__dirname, '/api-db.json'))
router.db._.id = 'key'
// router.db._.foreignKeySuffix = 'Key'
const middlewares = jsonServer.defaults()

// Set default middlewares (logger, static, cors and no-cache)
server.use(middlewares)

// To handle POST, PUT and PATCH you need to use a body-parser
// You can use the one used by JSON Server
server.use(jsonServer.bodyParser)

// Rewrite routes
server.use(jsonServer.rewriter({
  '/api/jwt': '/datasets',
  '/api/jwt/:key': '/datasets/:key',
  '/api/jwt/:key/contact': '/datasets/:key/contacts' // '/contacts?datasetKey=:key'
}))

// Process put request on a list of contacts
server.put('/datasets/:key/contacts', function (req, res) {
  if (!isAuthorized(req)) {
    res.sendStatus(401)
    return
  }

  const datasetKey = req.url.match(/^\/datasets\/([^\/]+)\/contacts$/)[1]

  // See https://github.com/typicode/lowdb
  // check if the dataset exists and is custom
  var dataset = router.db
    .get('datasets')
    .find({key: datasetKey})
    .value()

  if (dataset && dataset.is_custom) {
    console.log('found dataset ' + datasetKey)
    router.db
      .get('contacts')
      .find({datasetId: datasetKey})
      .assign({contacts: setContactIds(req.body)})
      .write()
    // return the persisted contacts list
    contactsObj = router.db
      .get('contacts')
      .find({datasetId: datasetKey})
      .value()
    res.status(200).jsonp(contactsObj.contacts)
  } else {
    res.sendStatus(404)
  }
})

// Change payload before saving
server.use((req, res, next) => {
  if (!isAuthorized(req)) {
    res.sendStatus(401)
    return
  }

  if (req.method === 'POST' && req.url === '/datasets') {
    // when creating a new dataset, create an empty list of contacts
    var datasetKey = req.body.key
    router.db
      .get('contacts')
      .upsert({datasetId: datasetKey, contacts: []})
      .write()
  }

  // Continue to JSON Server router, delay for real experience
  setTimeout(function () {
    next()
  }, 500)
})

// Finetune output
router.render = (req, res) => {
  if (req.method === 'GET' && req.url.match("/contacts")) {
    // Only return the actual contacts
    res.jsonp(res.locals.data[0].contacts)
  } else {
    res.jsonp(res.locals.data)
  }
}

// Use default router
server.use(router)
server.listen(8081, () => {
  console.log('JSON Server is running on port 8081')
})

// Helper Functions

function isAuthorized (req) {
  return req.headers.authorization === 'JWT xxx'
}

function setContactIds (contacts) {
  var newContacts = JSON.parse(JSON.stringify(contacts))
  var idCounter = 0
  for (let i = 0, j = newContacts.length; i < j; i++) {
    if (newContacts[i].id > idCounter) {
      idCounter = newContacts[i].id
    }
  }
  for (let i = 0, j = newContacts.length; i < j; i++) {
    if (!newContacts[i].id) {
      newContacts[i].id = ++idCounter
    }
  }
  return newContacts
}
