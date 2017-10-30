/*
var serialport = require('serialport');
var portName = 'COM4';
var sp = new serialport.SerialPort(portName, {
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false,
    parser: serialport.parsers.Readline("\r\n")
});

sp.on('data', function(input) {
    console.log(input);
});
*/



/*
	Express.js GET/POST example
	Shows how handle GET, POST, PUT, DELETE
	in Express.js 4.0
	created 14 Feb 2016
	by Tom Igoe
*/

var express = require('express');			// include express.js
var app = express();						// a local instance of it
var bodyParser = require('body-parser');	// include body-parser
var WebSocketServer = require('ws').Server	// include Web Socket server

// you need a  body parser:
app.use(bodyParser.urlencoded({extended: false})); // for application/x-www-form-urlencoded

// this runs after the server successfully starts:
function serverStart() {
	var port = server.address().port;
	console.log('Server listening on port '+ port);
}

app.get('/chunked', function(request, response) {
	response.write('\n');
	response.write('           `:;;;,`                      .:;;:.           \n');
	response.write('        .;;;;;;;;;;;`                :;;;;;;;;;;:     TM \n');
	response.write('      `;;;;;;;;;;;;;;;`            :;;;;;;;;;;;;;;;      \n');
	response.write('     :;;;;;;;;;;;;;;;;;;         `;;;;;;;;;;;;;;;;;;     \n');
	response.write('    ;;;;;;;;;;;;;;;;;;;;;       .;;;;;;;;;;;;;;;;;;;;    \n');
	response.write('   ;;;;;;;;:`   `;;;;;;;;;     ,;;;;;;;;.`   .;;;;;;;;   \n');
	response.write('  .;;;;;;,         :;;;;;;;   .;;;;;;;          ;;;;;;;  \n');
	response.write('  ;;;;;;             ;;;;;;;  ;;;;;;,            ;;;;;;. \n');
	response.write(' ,;;;;;               ;;;;;;.;;;;;;`              ;;;;;; \n');
	response.write(' ;;;;;.                ;;;;;;;;;;;`      ```       ;;;;;`\n');
	response.write(' ;;;;;                  ;;;;;;;;;,       ;;;       .;;;;;\n');
	response.write('`;;;;:                  `;;;;;;;;        ;;;        ;;;;;\n');
	response.write(',;;;;`    `,,,,,,,,      ;;;;;;;      .,,;;;,,,     ;;;;;\n');
	response.write(':;;;;`    .;;;;;;;;       ;;;;;,      :;;;;;;;;     ;;;;;\n');
	response.write(':;;;;`    .;;;;;;;;      `;;;;;;      :;;;;;;;;     ;;;;;\n');
	response.write('.;;;;.                   ;;;;;;;.        ;;;        ;;;;;\n');
	response.write(' ;;;;;                  ;;;;;;;;;        ;;;        ;;;;;\n');
	response.write(' ;;;;;                 .;;;;;;;;;;       ;;;       ;;;;;,\n');
	response.write(' ;;;;;;               `;;;;;;;;;;;;                ;;;;; \n');
	response.write(' `;;;;;,             .;;;;;; ;;;;;;;              ;;;;;; \n');
	response.write('  ;;;;;;:           :;;;;;;.  ;;;;;;;            ;;;;;;  \n');
	response.write('   ;;;;;;;`       .;;;;;;;,    ;;;;;;;;        ;;;;;;;:  \n');
	response.write('    ;;;;;;;;;:,:;;;;;;;;;:      ;;;;;;;;;;:,;;;;;;;;;;   \n');
	response.write('    `;;;;;;;;;;;;;;;;;;;.        ;;;;;;;;;;;;;;;;;;;;    \n');
	response.write('      ;;;;;;;;;;;;;;;;;           :;;;;;;;;;;;;;;;;:     \n');
	response.write('       ,;;;;;;;;;;;;;,              ;;;;;;;;;;;;;;       \n');
	response.write('         .;;;;;;;;;`                  ,;;;;;;;;:         \n');
	response.write('                                                         \n');
	response.write('                                                         \n');
	response.write('                                                         \n');
	response.write('                                                         \n');
	response.write('    ;;;   ;;;;;`  ;;;;:  .;;  ;; ,;;;;;, ;;. `;,  ;;;;   \n');
	response.write('    ;;;   ;;:;;;  ;;;;;; .;;  ;; ,;;;;;: ;;; `;, ;;;:;;  \n');
	response.write('   ,;:;   ;;  ;;  ;;  ;; .;;  ;;   ,;,   ;;;,`;, ;;  ;;  \n');
	response.write('   ;; ;:  ;;  ;;  ;;  ;; .;;  ;;   ,;,   ;;;;`;, ;;  ;;. \n');
	response.write('   ;: ;;  ;;;;;:  ;;  ;; .;;  ;;   ,;,   ;;`;;;, ;;  ;;` \n');
	response.write('  ,;;;;;  ;;`;;   ;;  ;; .;;  ;;   ,;,   ;; ;;;, ;;  ;;  \n');
	response.write('  ;;  ,;, ;; .;;  ;;;;;:  ;;;;;: ,;;;;;: ;;  ;;, ;;;;;;  \n');
	response.write('  ;;   ;; ;;  ;;` ;;;;.   `;;;:  ,;;;;;, ;;  ;;,  ;;;;   \n');
	response.write('\n');
	response.end();
});

// this is the POST handler:
app.all('/*', function (request, response) {
	console.log('Got a ' + request.method + ' request');
	// the parameters of a GET request are passed in
	// request.body. Pass that to formatResponse()
	// for formatting:
	console.log(request.headers);
	if (request.method == 'GET') {
		console.log(request.query);
	} else {
		console.log(request.body);
	}

	// send the response:
	response.send('OK');
	response.end();
});

// start the server:
var server = app.listen(8080, serverStart);

// create a WebSocket server and attach it to the server
var wss = new WebSocketServer({server: server});

wss.on('connection', function connection(ws) {
	// new connection, add message listener
	ws.on('message', function incoming(message) {
		// received a message
		console.log('received: %s', message);

		// echo it back
		ws.send(message);
	});
});






/*
var express = require('express');
//var routes = require('./routes');
var http = require('http');

var app = express();
var server = app.listen(3000);
var io = require('socket.io').listen(server)

app.cnofigure('development', function() {
	app.use(express.errorHandler());
});

//app.get('/', routes.index);

console.log("express server listening on port 3000");

io.sockets.on('connection', function(socket) {
	console.log('A new user connected!');
	socket.emit('info', {msg: 'Test tes test' });
});*/