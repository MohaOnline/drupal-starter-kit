
SHORTENING A URL:
-------------------------------
Default format is JSON:  
http://lb.cm/shurly/api/shorten?longUrl=http://www.lullabot.com

Text format returns just the short URL:
http://lb.cm/shurly/api/shorten?longUrl=http://www.lullabot.com&format=txt

XML format:  
http://lb.cm/shurly/api/shorten?longUrl=http://www.lullabot.com&format=xml

PHP serialized array:  
http://lb.cm/shurly/api/shorten?longUrl=http://www.lullabot.com&format=php

JSONP takes (optional) additional "callback" argument to define function:  
http://lb.cm/shurly/api/shorten?longUrl=http://www.lullabot.com&format=jsonp&callback=gimmeUrl

API Keys:  
Users can create API keys and use them to associate a shortening request with their account. Additionally, their roles will be honored and the associated rate limiting will be used.
http://lb.cm/shurly/api/shorten?longUrl=http://www.lullabot.com&apiKey=84a29ac36f0507b7b98672a9d13a2e46_A

Additionally, a user's API key can be retrieved programmatically if the browser is logged in.
http://lb.cm/shurly/api/key


EXPANDING A URL:
-------------------------------
Works just as above, but returns expanded URL. All formats above are supported.
Here, the API key only modifies rate limiting.

http://lb.cm/shurly/api/expand?shortUrl=http://lb.cm/Zk5