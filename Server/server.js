var http = require('http')
var director = require('director');

var router = new director.http.Router()

colors = ['red', 'green', 'blue', 'yellow', 'magenta', 'white', 'orange']
wires = ['C', 'D', 'R', 'G', 'B', 'D', 'D', 'R', 'B', 'G', 'C', 'B', 'G', 'G', 'C', 'R']

router.get('/start', function(req, res) {

	var gameColors = colors.filter(function(color) {
		return Math.random() > 0.75
	})	

	if (gameColors.length == 0) {
		gameColors.push(colors[Math.floor(Math.random() * colors.length)])
	}

	var lightMap = 0;
	gameColors.forEach(function(color) {
		var i = colors.indexOf(color)
		if (i >= 0) {
			lightMap += Math.pow(2, 7-i)
		}
	})

	var wireMap = 0
	var gameWires = []

	for (var i = 0; i < 5; i++) {
		if (Math.random() > 0.3) {
			gameWires.push(Math.floor(Math.random() * 16))	
		} else {
			gameWires.push(0)
		}
	}

	gameWires.forEach(function(action, index) {
		var cut = 1

		if (wires[action] == 'D') 
			cut = 0

		if (wires[action] == 'R' && gameColors.indexOf('red') < 0)
			cut = 0

		if (wires[action] == 'G' && gameColors.indexOf('green') < 0)
			cut = 0
	
		if (wires[action] == 'B' && gameColors.indexOf('blue') < 0)
			cut = 0

		if (cut == 1)
			wireMap += Math.pow(2, 7-index)
	})


	this.res.writeHead(200, {'Content-Type': 'application/json'})
	this.res.end(gameColors.toString())
})



var server = http.createServer(function (req, res) {
	router.dispatch(req, res, function (err) {
		if (err) {
			res.writeHead(404)
			res.end()
		}
	});
});

server.listen(8080)
