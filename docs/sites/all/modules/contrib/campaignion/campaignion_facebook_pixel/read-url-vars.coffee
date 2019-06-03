eventCodes =
  l: 'Lead'
  r: 'CompleteRegistration'
  v: 'ViewContent'

readPixelFragment = (part, pixels) ->
  pixels = pixels || Drupal.settings.campaignion_facebook_pixel.pixels

  for pixelStr in part.split('&')
    p = pixelStr.indexOf('=')
    pixelId = pixelStr.substring(0, p)
    if pixelId not of pixels
      pixels[pixelId] = []
    for code in pixelStr.substring(p + 1).split(',')
      pixels[pixelId].push(if code of eventCodes then eventCodes[code] else code)
    pixels[pixelId] = pixels[pixelId].filter((value, index, self) -> self.indexOf(value) == index)
  return pixels

readFragmentParts = (hash) ->
  hash = hash || window.location.hash.substr(1)
  if not hash
    return ''

  new_parts = []
  for part in hash.split(';')
    if part.substr(0, 4) == 'fbq:'
      readPixelFragment(part.substring(4))
    else
      new_parts.push(part)
  
  return new_parts.join(';')
  

send = () ->
  for pixelId, events of Drupal.settings.campaignion_fb_pixel.pixels
    fbq('init', pixelId)
    for e in events
      fbq('trackSingle', pixelId, e)
  return

if not Drupal.settings.campaignion_fb_pixel?.pixels?
  Drupal.settings.campaignion_fb_pixel = { pixels: {} }
hash = window.location.hash.substr(1)
if hash
  newHash = readFragmentParts(hash)
  if newHash != hash
    window.location.hash = '#' + newHash

send()
return
