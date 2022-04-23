$ = jQuery

if Drupal.formatNumber?
  formatNumber = (now) -> Drupal.formatNumber(now, 0)
else
  formatNumber = (now) ->
    num = ''
    # Add thousand separators to the number
    zeros = 0
    now = Math.round(now)
    if now == 0
      return '0'
    while now > 0
      while zeros > 0
        num = '0' + num
        zeros -= 1
      rest = now % 1000
      zeros = 3 - rest.toString().length
      num = rest + ',' + num
      now = (now - rest) / 1000

    # cut last thousand separator from output.
    return num.slice(0, num.length - 1)


class PgbarItem
  constructor: (@settings, wrapper) ->
    @wrapper = wrapper
    @current = 0
    @counter = $('.pgbar-counter', wrapper)
    @bars = $('.pgbar-current', wrapper)
    @target = $('.pgbar-target', wrapper)
    @target.html(formatNumber(@settings.target))
    @needed = $('.pgbar-needed', wrapper)
    if @settings.extractor
      @extractor = @settings.extractor
    else if @settings.find_at
      @extractor = (data) =>
        parts = @settings.find_at.split('.').filter((i) => i != '')
        d = data
        # walk the path defined by parts
        for p in parts
          if d[p]
            d = d[p]
          else
            # nothing found: return 0
            return 0
        if typeof d == "number" || typeof d == "string"
          value = parseInt(d, 10)
          if !isNaN(value)
            return value
        # return 0 if we did not find a number
        return 0
    else
      @extractor = (data) =>
        return parseInt(data.pgbar[@settings.field_name][@settings.delta])

  selectTarget: (current) ->
    t = 1
    # copy the array
    targets = @settings.targets.concat()
    while targets.length > 0
      t = targets.shift()
      if (current * 100 / t) <= parseInt(@settings.threshold, 10)
        return t
    return t

  poll: ->
    registry = Drupal.behaviors.polling.registry
    callback = (data) =>
      to_abs = @extractor(data)
      @animate(to_abs) if to_abs != @current
      return
    registry.registerUrl(
      @settings.pollingURL,
      @settings.id,
      callback
    )

  animate: (to_abs, from_abs = @current) ->
    target = @settings.target
    best_target = @selectTarget(to_abs)
    if best_target != target
      target = best_target
      @target.html(formatNumber(target))
      @needed.html(formatNumber(target - to_abs))
    if @settings.inverted
      from = 1 - from_abs / target
      to = 1 - to_abs / target
      diff = from - to
    else
      from = from_abs / target
      to = to_abs / target
      diff = to - from

    @counter.html(formatNumber(from_abs))

    duration = 500 + 1000 * diff
    resetCounters = (num, fx) =>
      @counter.html(formatNumber(num))

    if @settings.vertical
      @bars.height(from * 100 + '%')
      @bars.animate({height: to * 100 + '%'}, {duration: duration})
    else
      @bars.width(from * 100 + '%')
      @bars.animate({width: to * 100 + '%'}, {duration: duration})
    @wrapper.animate({val: to_abs}, {duration: duration, step: resetCounters})

    @current = to_abs

  animateInitially: ->
    animation = => @animate(@settings.current)
    window.setTimeout(animation, 2000)

PgbarItem.fromElement = ($element) ->
  id = $element.attr('id')
  settings = Drupal.settings.pgbar[id]
  settings['id'] = id
  settings['inverted'] = $element.data('pgbarInverted')
  settings['vertical'] = $element.data('pgbarDirection') == 'vertical'
  return new PgbarItem(settings, $element)

Drupal.behaviors.pgbar = {}
Drupal.behaviors.pgbar.attach = (context, settings) ->
  $('.pgbar-wrapper[id]', context).each(->
    item = PgbarItem.fromElement($(this))

    # Do not animate initially for an external source with initial count 0.
    # This doubles the animation due to the timeout timings between pgbar and
    # polling.
    if item.settings['external_url']
      item.poll()
    else if item.settings['autostart']
      item.animateInitially()
      item.poll()
  )
