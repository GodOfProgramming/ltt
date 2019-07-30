function init() {
  function authToken() {
    return JSON.parse(localStorage.getItem("ls.authToken"));
  }

  function send(method, url, params, onSuccess, onFailure, onComplete) {
    var http = new XMLHttpRequest();
    http.open(method, url, true);
    http.setRequestHeader("Content-Type", "application/json");
    http.setRequestHeader("Authorization", `Bearer ${authToken()}`);
    http.send(JSON.stringify(params));

    http.onreadystatechange = function(e) {
      if (this.readyState == 4) {
	if (this.status == 200) {
	  if (onSuccess) {
	    onSuccess(JSON.parse(http.responseText));
	  }
	} else {
	  if (onFailure) {
	    onFailure(JSON.parse(http.responseText));
	  }
	}

	if (onComplete) {
	  onComplete(JSON.parse(http.responseText));
	}
      }
    };
  }

  chrome.buildpay = new Object();
  var buildpay = chrome.buildpay;

  buildpay.switchCreds = function(email) {
    var method = 'POST';
    var url = 'http://localhost:51034/switch_credentials';
    var params = { email: email };
    var onSuccess = function(resp) { 
      console.log('Success');

      permissions = JSON.stringify(resp.permissions);
      user = JSON.stringify(resp.user);
      auth = JSON.stringify(resp.auth_token);

      localStorage.setItem("ls.currentUser", user)
      localStorage.setItem("ls.systemPermissions", permissions);
      localStorage.setItem("ls.authToken", auth);
    };
    var onFailure = function(resp) { 
      console.log('Failure'); 
      console.log(resp);
    };
    send(method, url, params, onSuccess, onFailure);
  };

  buildpay.whoami = function() {
    console.log(JSON.parse(localStorage.getItem('ls.currentUser')).email);
  };
}

var script = document.createElement('script');
var code = document.createTextNode(`(${init})();`);
script.appendChild(code);
(document.body || document.head || document.documentElement).appendChild(script);

