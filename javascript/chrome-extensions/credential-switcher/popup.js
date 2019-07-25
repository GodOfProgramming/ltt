document.addEventListener("DOMContentLoaded", function() {
  var input = document.getElementById("credential_switcher_email");

  function switchCredentials() {
    var email = input.value;
    var url = 'http://localhost:51034/switch_credentials';
    var params = `email=${email}`;
    var http = new XMLHttpRequest();
    http.open("POST", url, true);
    console.log("Params:", params);
    http.send(params);

    http.onreadystatechange = function(e) {
      if (this.readyState == 4) {
	if (this.status == 200) {
	  console.log("Success");
	} else {
	  console.log("Failure");
	}
	console.log("Local Storage", localStorage);
      }
    }
  }

  if (input) {
    console.log("Input found");
    input.addEventListener("keyup", function(event) {
      if (event.keyCode === 13) {
	event.preventDefault();
	switchCredentials();
      }
    });
  } else {
    console.log("Input not found");
  }
});
