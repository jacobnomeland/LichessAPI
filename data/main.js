var gateway = `ws://${window.location.hostname}/ws`; // The gateway is the entry point to the WebSocket interface.
// window.location.hostname gets the current page address (the web server IP address).
var websocket;
window.addEventListener('load', onLoad); // event listener when web page loads
if (!!window.EventSource) {
  var source = new EventSource('/events');
  source.addEventListener('myevent', function(e) {
    console.log("myevent", e.data);
  }, false);
}

function initWebSocket() { // initializes a websocket connection on the gateway
  console.log('Trying to open a WebSocket connection...');
  websocket = new WebSocket(gateway);
  websocket.onopen    = onOpen;
  websocket.onclose   = onClose;
  websocket.onmessage = onMessage; // <-- add this line
}
function onOpen(event) { // when connection is opened simply print message saying connection opened
  console.log('Connection opened');
}
function onClose(event) { // if for some reason web socket connection closed, we call the initt websocket after xxxx seconds
  console.log('Connection closed');
  setTimeout(initWebSocket, 2000);
}
function onMessage(event) { // we will recieve a message from server (esp32) and it will send either 1 or 0
  console.log('inside onmessage funciton');
  var obj = JSON.parse(event.data);

  var state;

  if(obj.hasOwnProperty('setUsername')){
    document.getElementById('username').innerHTML = obj.setUsername;
  }
  
  if (event.data == "1"){
    state = "ON";
  }
  else{
    state = "OFF";
  }
  document.getElementById('state').innerHTML = state;
}
function onLoad(event) { // when called initialize the following functions:
  initWebSocket();
  initButton();
  onMessage(event); // apparently this is needed to show the inital information on load time, this was not in the origional code
}
function initButton() { // adds eventlistener to the button as a toggle
  document.getElementById('button').addEventListener('click', toggle);
  document.getElementById('submitMove').addEventListener('click', makeChessMove);
}
function toggle(){ //  sends a message using the wbsocket connection with the 'toggle' text to the server? (esp32)
  websocket.send('toggle');
  websocket.send('{ "firstName":"John" , "lastName":"Doe" }');
}

function makeChessMove(){
  websocket.send(document.getElementById('chessMoveTxt').value);
  var obj = new Object();
  obj.chessMoveTxt = document.getElementById('chessMoveTxt').value;
  websocket.send(JSON.stringify(obj));
}

