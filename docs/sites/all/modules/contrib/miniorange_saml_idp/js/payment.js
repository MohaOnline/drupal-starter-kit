/*
function payment(plan, username, hostName){
  var loginUrl = hostName + "/moas/login";
  var paymentUrl = hostName + "/moas/initializepayment";
  
  var f = document.createElement("form");
  f.setAttribute('method',"post");
  f.setAttribute("action", loginUrl);
  f.setAttribute("target", "_blank");

  var i = document.createElement("input"); 
  i.setAttribute('type',"text");
  i.setAttribute('name',"username");
  i.setAttribute('value', username);

  var i2 = document.createElement("input");
  i2.setAttribute("type", "text");
  i2.setAttribute("name", "redirectUrl");
  i2.setAttribute("value", paymentUrl);

  var i3 = document.createElement("input"); 
  i3.setAttribute('type',"text");
  i3.setAttribute('name',"requestOrigin");
  i3.setAttribute('value', plan);

  f.appendChild(i);
  f.appendChild(i2);
  f.appendChild(i3);
   document.body.appendChild(f);

  f.submit();
}*/
