module.exports = {
  default_redirect_url: 'http://old-default-url.com', // This could be the old redirect url.
  redirects: [],
  fields: [
    {
      id: 'f_name',
      label: 'First name'
    },
    {
      id: 'l_name',
      label: 'Last name'
    },
    {
      id: 'email',
      label: 'Email address'
    }
  ],
  endpoints: {
    nodes: '/api/getnodes',
    redirects: '/api/data'
  }
}
