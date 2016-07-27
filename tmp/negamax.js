;(function() {

    // width/height of canvas
    var mapSize = 300;

    // 2d array store in single dimensional array
    var map = []; 
    
    // max blocks to draw
    var mapLength = 10;
    var blockSize = mapSize / mapLength;
    var blockMargin = blockSize/blockSize;

    const moves = {west: -1, north: -mapSize, east: 1, south: mapSize};
    const mapMoves = [moves.west, moves.north, moves.east, moves.south];
    const mapMovesX = [-1, 0, 1, 0];
    const mapMovesY = [0, -1, 0, 1];
    const mapTypes = {wall: -1, empty: 0};

    // create canvas and get context
    var canvas = createCanvas(mapSize, mapSize);
    var ctx = canvas.getContext('2d');

    // initialize map
    for (var x = 0; x < mapLength; x++) {
        for (var y = 0; y < mapLength; y++) {
            map[getID(x, y)] = 0;
        }
    }

    var player = new Player(1, mapLength-2, 2, '#333');
    player.draw();

    // create and draw boundaries
    for (var i = 0; i < mapLength; i++) {

        addWall(i, 0);
        addWall(i, mapLength - 1);
        addWall(0, i);
        addWall(mapLength - 1, i);

        // ctx.lineWidth = 0.3;
        // ctx.strokeStyle = '#222';
        // ctx.beginPath();
        // ctx.moveTo(0, i * blockSize);
        // ctx.lineTo(mapLength * blockSize, i * blockSize);
        // ctx.stroke();
        // ctx.beginPath();
        // ctx.moveTo(i * blockSize, 0);
        // ctx.lineTo(i * blockSize, mapLength * blockSize);
        // ctx.stroke();
    }

    function tick()
    {
    }

    function Player(x, y, move, color)
    {
        this.x = x || 0;
        this.y = y || 0;
        this.move = move || moves.idle;
        this.color = color;
        this.die = false;
        this.draw = function() {
            drawCell(this.x, this.y, this.color);
        }
        this.update = function() {
            this.x += mapMovesX[this.move]; 
            this.y += mapMovesY[this.move]; 
        }
    }

    // get 1D position of 2D values 
    function getID(x, y)
    {
        return x + y * mapLength;
    }

    function addWall(x, y) 
    {
        map[getID(x, y)] = mapTypes.wall;
        drawCell(x, y, '#191919');
    }

    function drawCell(x, y, color) 
    {
        var x = x * blockSize;
        var y = y * blockSize;
        var width = blockSize - blockMargin;
        var height = blockSize - blockMargin;
        ctx.fillStyle = color || '#000';
        ctx.fillRect(x, y, width, height);
    }

    function addPlayer(x, y, value, color)
    {
        map[getID(x, y)] = value;
        drawCell(x, y, color);
    }

    function createCanvas(width, height)
    {
        var canvas = document.createElement('canvas');
        canvas.width = width;
        canvas.height = height;

        var resizeCanvas = function resizeCanvas()
        {
            var screenSize = Math.min(window.innerWidth, window.innerHeight);
            canvas.style.window = screenSize;
            canvas.style.height = screenSize;
        };

        // append to body
        document.body.appendChild(canvas);

        // update size on screen resize
        window.addEventListener('resize', resizeCanvas);

        resizeCanvas();

        return canvas;
    }

})();

