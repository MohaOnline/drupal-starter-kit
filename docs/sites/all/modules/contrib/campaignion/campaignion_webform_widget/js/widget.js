var messageParent = function(scrollTop){
  // be sure this code runs when document.body is defined
  var h = document.body.scrollHeight;
  h = (scrollTop)? h+'s':h;
  if(top.postMessage){
    top.postMessage( h , '*');
  } else {
    window.location.hash = 'h'+h;
  }
}

function parseQueryString(string) {
  if (typeof string !== "string") {
    string = "";
  }
  var vars = string.split('&');
  var params = {};
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (!pair[0]) {
      continue;
    }
    params[decodeURIComponent(pair[0])] = decodeURIComponent(pair[1]);
  }
  return params;
}
function serializeToQueryString(obj) {
  var str = [];
  for(var p in obj) {
    if (obj.hasOwnProperty(p)) {
      str.push(encodeURIComponent(p) + "=" + encodeURIComponent(obj[p]));
    }
  }
  return str.join("&");
}

function gaLinkerHandler() {
  var query = parseQueryString(document.location.search.substring(1));
  var galinker = query['_ga'];

  if (galinker) {
    var form = document.getElementsByTagName("form")[0];
    var url = document.createElement('a');
    url.href = form.action;
    var action = parseQueryString(url.search.substring(1))
    action['_ga'] = galinker;
    url.search = '?' + serializeToQueryString(action);
    form.action = url.href;
  }
}

function registerListeners() {
  if (window.addEventListener) {
    window.addEventListener("DOMSubtreeModified", function(){messageParent();}, true);
    document.addEventListener("DOMContentLoaded", gaLinkerHandler);
  } else if (window.attachEvent) {
    window.attachEvent("onDOMSubtreeModified", function(){messageParent();});
  }
}
window.onload = function() {
  messageParent();
  registerListeners();
}
window.onresize = function() {
  messageParent();
}
