const path = require('path')
const fakeNodes = require('../test/fixtures/node-list.js')
const initialData = require('../test/fixtures/initial-data.js')
const jsonServer = require('json-server')
const server = jsonServer.create()
const router = jsonServer.router(path.join(__dirname, '/api-db.json'))
const middlewares = jsonServer.defaults()

// Set default middlewares (logger, static, cors and no-cache)
server.use(middlewares)

// Add custom routes before JSON Server router
server.get('/getnodes', (req, res) => {
  res.status(200).jsonp(fakeNodes(req.url))
})

// To handle POST, PUT and PATCH you need to use a body-parser
// You can use the one used by JSON Server
server.use(jsonServer.bodyParser)

// Change payload before saving
server.use((req, res, next) => {
  if (req.method === 'PUT') {
    // Do stuff
  }
  // Continue to JSON Server router
  next()
})

// Use default router
server.use(router)
server.listen(8081, () => {
  console.log('JSON Server is running on port 8081')
})
