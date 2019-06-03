$ = jQuery

class PollingEndpoint
  constructor: (settings) ->
    defaults =
      url: ''
      interval: 5000
      intervalMultiplier: 1.07
      maxErrorCount: 15
    @settings = $.extend({}, defaults, settings)
    @interval = @settings.interval
    @errorCount = 0
    @callbacks = {}
    @stopped = false

  scheduleNextPoll: ->
    # Set the next timeout.
    setTimeout(=>
      @poll()
      if !@stopped
        # Increase the interval.
        @interval = Math.floor(@interval * @settings.intervalMultiplier)
        @scheduleNextPoll()
    , @interval)

  poll: ->
    return if @stopped

    jQuery.ajax({
      url: @settings.url,
      success: (data) =>
        # Call all callbacks with the data.
        callback(data) for _, callback of @callbacks
        return

      error: (data) =>
        # on 403 or 404 do *not* set a next polling try
        if data.status == '403' # forbidden
          @stopped = true
        else if data.status == '404' # not found
          @stopped = true
        else if @errorCount >= @settings.maxErrorCount
          @stopped = true
        else
          @errorCount += 1
        return
    })

  addCallback: (key, callback) ->
    @callbacks[key] = callback
    return @


class EndpointRegistry
  constructor: ->
    @registry = {}

  registerUrl: (url, key, callback) ->
    if not @registry[url]
      @registry[url] = new PollingEndpoint({url: url})
    @registry[url].addCallback(key, callback)

  start: ->
    for url, endpoint of @registry
      endpoint.scheduleNextPoll()
    return


Drupal.behaviors.polling =
  registry: new EndpointRegistry()
  attach: (context, settings) ->
    if $('html', context).length
      defaults =
        initialTimeout: 500,
      settings = $.extend({}, defaults, settings.polling)
      start = => @registry.start()
      setTimeout(start, settings.initialTimeout)
