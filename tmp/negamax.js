;(function() {

    // width/height of canvas
    var mapSize = 300;

    // 2d array store in single dimensional array
    var map = []; 
    
    // max blocks to draw
    var mapLength = 10;
    var blockSize = mapSize / mapLength;
    var blockMargin = blockSize/blockSize;

    const mapMoves = [
        // west, 
        -1, 
        // north, 
        -mapLength, 
        // east, 
        1, 
        //south
        mapLength,
    ];
    const moves = {idle: 4, west: 0, north: 1, east: 2, south: 3};
    const mapMovesX = [-1, 0, 1, 0];
    const mapMovesY = [0, -1, 0, 1];
    const mapTypes = {wall: 1, empty: 0};

    // create canvas and get context
    var canvas = createCanvas(mapSize, mapSize);
    var ctx = canvas.getContext('2d');

    // initialize map
    for (var x = 0; x < mapLength; x++) {
        for (var y = 0; y < mapLength; y++) {
            map[getID(x, y)] = mapTypes.empty;
        }
    }

    // create and draw boundaries
    for (var i = 0; i < mapLength; i++) {

        addWall(i, 0);
        addWall(i, mapLength - 1);
        addWall(0, i);
        addWall(mapLength - 1, i);

        ctx.lineWidth = 0.3;
        ctx.strokeStyle = '#222';
        ctx.beginPath();
        ctx.moveTo(0, i * blockSize);
        ctx.lineTo(mapLength * blockSize, i * blockSize);
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(i * blockSize, 0);
        ctx.lineTo(i * blockSize, mapLength * blockSize);
        ctx.stroke();
    }

    var MAX_PLAYERS = 2;
    var MAXN_DEPTH = 6;//mapLength * 3;

    var players = [];
    var startPositions = [
        {x: 1, y: mapLength-2, move: moves.east, color: '#333'},
        {x: mapLength-2, y: 1, move: moves.west, color: '#999'},
        {x: 1, y: 1, move: moves.east, color: 'green'},
        {x: mapLength-2, y: mapLength-2, move: moves.west, color: 'yellow'},
    ]
    for (var playerIndex = 0; playerIndex < MAX_PLAYERS; playerIndex++) {
        var curr = startPositions[playerIndex];
        players[playerIndex] = new Player(curr.x, curr.y, 'player'+(playerIndex+1), curr.move, curr.color);
        players[playerIndex].draw();
    }

    // registreKeyboard(player1);
    window.map = map;

    window.iterations = {
        negamax: 0,
        floodfill: 0,
        evaluatePosition: 0,
    };

    // tick();

    window.addEventListener('click', function() {
        tick();
    })

    // criar cache dos distMap
    // - com 2 jogadores, eh executado floodfill 4x, sendo que seria preciso 2x(1 pra cada jogador)
    // o id do cache poderia ser o "tick" atual mais o id|pos do player
    //                                \_ um sequencial para cada iteracao
    function floodfill(distMap, id)
    {
        window.iterations.floodfill++;
        var q = [id], q2 = [];
        distMap[id] = 1;
        var dist = 1;
        while(q.length) {
            dist++;
            for(var i=0;i<q.length;i++) {
                var id = q[i];
                for(var move=0;move<4;move++) {
                    var nextId = id+mapMoves[move];
                    // safe recursive
                    if (nextId < 0 || nextId > mapLength * mapLength) continue;
                    if(map[nextId] || distMap[nextId])
                        continue;
                    distMap[nextId] = dist;
                    q2.push(nextId);
                }
            }
            q = q2; q2 = [];
        }
        return distMap; 
    }

    function evaluatePosition(alphaPosition, betaPosition) 
    {
        window.iterations.evaluatePosition++;
        var score = 0;

        var alphaDistMap = floodfill([], alphaPosition);
        var betaDistMap = floodfill([], betaPosition);

        // drawDistMap(alphaDistMap, 'blue');
        // drawDistMap(betaDistMap, 'red');

        // first and last rows are walls
        for(var i = mapLength + 1; i < mapLength * (mapLength - 1) - 1; i++) {
            //wall
            if(map[i]) continue; 

            if (alphaDistMap[i] && betaDistMap[i]) {
                var diff = alphaDistMap[i] - betaDistMap[i];
                if (diff < 0) {
                    score++;
                } else {
                    score--;
                }
                continue;
            }

            if (alphaDistMap[i]) score++;
            if (betaDistMap[i]) score--;
        }

        return score;
    }

    function evaluatePositions(positions, playerIndex) 
    {
        window.iterations.evaluatePosition++;

        var distMaps = [];
        var scores = [];
        for (var indexPosition = 0; indexPosition < positions.length; indexPosition++) {
            distMaps[indexPosition] = floodfill([], positions[indexPosition]);
            scores[indexPosition] = 0;
        }

        for (var indexPosition = 0; indexPosition < positions.length; indexPosition++) {

            if (indexPosition == playerIndex) continue;

            // ignroe first and last rows, are walls
            for(var i = mapLength + 1; i < mapLength * (mapLength - 1) - 1; i++) {

                //wall
                if(map[i]) continue; 

                if (distMaps[playerIndex][i] && distMaps[indexPosition][i]) {
                    var diff = distMaps[playerIndex][i] - distMaps[indexPosition][i];
                    if (diff < 0) {
                        scores[playerIndex]++;
                        scores[indexPosition]--;
                    } else {
                        scores[playerIndex]--;
                        scores[indexPosition]++;
                    }
                    continue;
                }

                if (distMaps[playerIndex][i]) {
                    scores[playerIndex]++;
                    scores[indexPosition]--;
                }
                if (distMaps[indexPosition][i]) {
                    scores[playerIndex]--;
                    scores[indexPosition]++;
                }
            }
        }

        return scores;
    }

    function drawDistMap(map, color)
    {
        for (var i = 0; i < map.length; i++) {
            if (map[i] === undefined) continue;
            var x = i % mapLength;
            var y = Math.floor(i/mapLength);
            drawCell(x, y, color, map[i]);
        }
    }

    function negamax(alphaPosition, betaPosition, depth, alpha, beta, bestMove)
    {
        window.iterations.negamax++;
        if (depth == 0) return evaluatePosition(alphaPosition, betaPosition);

        for (var move = 0; move < 4; move++) {
            // proxima posicao
            var id = alphaPosition + mapMoves[move];
            // posicao ja ocupada
            if (map[id]) continue;

            // just for evaluate
            map[id] = mapTypes.wall;
            var score = -negamax(betaPosition, id, depth - 1, -beta, -alpha);
            map[id] = mapTypes.empty;
            if (score > alpha) {
                alpha = score;
                bestMove = move;
                // alpha-beta pruning
                if (alpha >= beta) break;
            }
        }

        if (depth == 6) return bestMove;
        return alpha;
    }

    // https://project.dke.maastrichtuniversity.nl/games/files/phd/Nijssen_thesis.pdf
    // http://web.cs.du.edu/~sturtevant/papers/multiplayergamesthesis.pdf
    function maxn(positions, depth, playerIndex, bestMove)
    {
        window.iterations.negamax++;

        if (depth == 0) return evaluatePositions(positions, playerIndex);

        var nextPlayerIndex = (playerIndex + 1)  % MAX_PLAYERS;
        var bestScore = [-Infinity, -Infinity];

        for (var move = 0; move < 4; move++) {

            // clone
            var _positions = positions.slice();
            _positions[playerIndex] += mapMoves[move];

            if (map[_positions[playerIndex]]) {
                continue;
            }

            // var pos = getXY(_positions[playerIndex]);
            // drawCell(pos.x, pos.y, 'red', map[_positions[playerIndex]]);

            // just for evaluate
            map[_positions[playerIndex]] = mapTypes.wall;
            var scores = maxn(_positions, depth - 1, nextPlayerIndex);
            map[_positions[playerIndex]] = mapTypes.empty;

            if (scores[playerIndex] > bestScore[playerIndex]) {
                bestScore = scores;
                bestMove = move;
            }
        }

        if (depth == MAXN_DEPTH) return bestMove;
        return bestScore;
    }

    function clear()
    {
        context.clearRect(0, 0, canvas.width, canvas.height); 
    }

    function tick()
    {
        var positions = [];
        for (var playerIndex = 0; playerIndex < MAX_PLAYERS; playerIndex++) {
            positions.push(players[playerIndex].pos());
        }

        console.log('tick', positions);

        for (var playerIndex = 0; playerIndex < MAX_PLAYERS; playerIndex++) {

            var player = players[playerIndex];
            player.move = maxn(positions, MAXN_DEPTH, playerIndex, player.move);
            player.update();
            player.draw();
        }
        
        var alive = 0;
        for (var playerIndex = 0; playerIndex < MAX_PLAYERS; playerIndex++) {
            if (players[playerIndex].alive) alive++
        }
        if (alive == 0) return false;

        // player1.move = negamax(player1.pos(), player2.pos(), 6, -1e6, 1e6, player1.move);
        // player1.move = maxn([player1.pos(), player2.pos()], MAXN_DEPTH, 0, player1.move);
        // player1.update();
        // player1.draw();

        // player2.move = negamax(player2.pos(), player1.pos(), 6, -1e6, 1e6, player2.move);
        // player2.move = maxn([player1.pos(), player2.pos()], MAXN_DEPTH, 1, player2.move);
        // player2.update();
        // player2.draw();

        // setTimeout(tick, 1000);

        // console.clear();
        console.log('negamax: ', window.iterations.negamax);
        console.log('floodfill: ', window.iterations.floodfill);
        console.log('evaluatePosition: ', window.iterations.evaluatePosition);

        window.iterations = {
            negamax: 0,
            floodfill: 0,
            evaluatePosition: 0,
        };

    }

    function Player(x, y, id, move, color)
    {
        this.x = x || 0;
        this.y = y || 0;
        this.id = id;
        this.move = move;
        this.color = color;
        this.alive = true;
        this.pos = function() {
            return getID(this.x, this.y);
        }
        this.draw = function() {
            drawCell(this.x, this.y, this.color);
        }
        this.update = function() {
            if (this.move == moves.idle || !this.alive) return false;
            var x = this.x + mapMovesX[this.move]; 
            var y = this.y + mapMovesY[this.move]; 
            if (map[getID(x, y)]) {
                this.alive = false;
                console.error('gameover', this.id);
                return false;
            }
            this.x = x; 
            this.y = y;
            map[this.pos()] = this.id;
        }
        map[this.pos()] = id;
    }

    // get 1D position of 2D values 
    function getID(x, y)
    {
        return x + y * mapLength;
    }

    // get 2D position of 1D value
    function getXY(id)
    {
        return {
            x: id % mapLength,
            y: Math.floor(id/mapLength),
        }
    }

    function addWall(x, y) 
    {
        map[getID(x, y)] = mapTypes.wall;
        drawCell(x, y, '#191919');
    }

    function drawCell(x, y, color, value) 
    {
        var x = x * blockSize;
        var y = y * blockSize;
        var width = blockSize - blockMargin;
        var height = blockSize - blockMargin;

        ctx.clearRect(x, y, width, height); 

        ctx.fillStyle = color || '#000';
        ctx.fillRect(x, y, width, height);
        if (!value) return;

        // ctx.font = '30px Arial';
        ctx.fillStyle = '#fff';
        ctx.fillText(value, x + blockSize/3, y + blockSize/1.8);
    }

    function addPlayer(x, y, value, color)
    {
        map[getID(x, y)] = value;
        drawCell(x, y, color);
    }

    function registreKeyboard(player)
    {
        window.addEventListener('keydown', function(event) {
            if (player.alive) return;
            switch(event.keyCode) {

                case 37: // left
                case 38: // up
                case 39: // right
                case 40: // down
                    var move = event.keyCode - 37;
                    var pos = player.pos() + mapMoves[move];
                    if (!map[pos]) {
                        player.move = move;
                    }
                break;
            }
        })
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
