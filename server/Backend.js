/***************************** Prerequisites *****************************/
var Room = require("./Definitions.js");
const express = require('express')
const bodyParser = require('body-parser');
const app = express()

/***************************** Global Variables *****************************/
var rooms = [];

/***************************** Creating Rooms *****************************/

	// Room 1
	var Temperatures1 = [2700, 3500, 5000, 6000, 4200, 3500, 2200];
	var Lumens1 = [1500, 1700, 2000, 2200, 1900, 1700, 20];
	var Percentages1 = [0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.4];
	var Brightness1 = 1.0;
	var Compensation1 = [0.0, 0.0, 0.0];

	let room1 = new Room(1, Temperatures1, Lumens1, Percentages1, Brightness1, Compensation1);
	rooms.push(room1);

	// Room 2
	var Temperatures2 = [2700, 2700, 2700, 2700, 2700, 2700, 2700];
	var Lumens2 = [1700, 1700, 1700, 1700, 1700, 1700, 1700];
	var Percentages2 = [0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.4];
	var Brightness2 = 0.8;
	var Compensation2 = [0.0, 0.0, 0.0];

	let room2 = new Room(2, Temperatures2, Lumens2, Percentages2, Brightness2, Compensation2);
	rooms.push(room2);

/***************************** Http Requests *****************************/

/*
Inside /server
run:
	npm init
	npm install --save express
	npm install ejs --save
	npm install body-parser --save
	
to start: node Backend.js
*/

app.set('view engine', 'ejs')				//sets EJS as templating language in /views
app.use(express.static('public'));			//gives access to /public for access to CSS files
app.use(bodyParser.urlencoded({ extended: true }));	//for parsing of request body

app.get('/', function (req, res) {
  res.render('index', {weather: null, error: null});
})

app.post('/', function (req, res) {
  let city = req.body.city;
  let errStr = 'Error, please try again ' + city;
  res.render('index', {weather: null, error: errStr});
  
  //INFO FROM SENSOR
  
  /*let url = `http://api.openweathermap.org/data/2.5/weather?q=${city}&units=imperial&appid=${apiKey}`

  request(url, function (err, response, body) {
    if(err){
      res.render('index', {weather: null, error: 'Error, please try again'});
    } else {
      let weather = JSON.parse(body)
      if(weather.main == undefined){
        res.render('index', {weather: null, error: 'Error, please try again'});
      } else {
        let weatherText = `It's ${weather.main.temp} degrees in ${weather.name}!`;
        res.render('index', {weather: weatherText, error: null});
      }
    }
  });*/
})

app.listen(3000, function () {
  console.log('Example app listening on port 3000!')
})
