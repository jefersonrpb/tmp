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

    var player = new Player(1, mapLength-2, 'player', moves.east, '#333');
    // player.update();
    player.draw();

    registreKeyboard(player);

    window.map = map;

    var ai = new Player(mapLength-2, 1, 'ai', moves.west, '#999');
    // ai.update();
    ai.draw();

    window.iterations = {
        negamax: 0,
        floodfill: 0,
        evaluatePosition: 0,
        bestScore: [],
    };

    tick();

    console.log('player, ai, moves', player.move, ai.move);

    // criar cache dos distMap
    // - com 2 jogadores, eh executado floodfill 4x, sendo que seria preciso 2x(1 pra cada jogador)
    // o id do cache poderia ser o "tick" atual mais o id do player
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

        // clone
        var positions = positions.slice();

        if (depth == 0) return evaluatePositions(positions, playerIndex);

        var nextPlayerIndex = (playerIndex + 1)  % 2;
        var bestScore = [-Infinity, -Infinity];

        for (var move = 0; move < 4; move++) {

            var indexPosition = playerIndex;
            positions[indexPosition] += mapMoves[move];

            if (map[positions[indexPosition]]) continue;

            // just for evaluate
            map[positions[indexPosition]] = mapTypes.wall;
            var scores = maxn(positions, depth - 1, nextPlayerIndex);
            map[positions[indexPosition]] = mapTypes.empty;

            if (scores[playerIndex] > bestScore[playerIndex]) {
                bestScore = scores;
                bestMove = move;
            }
        }

        if (depth == 6) window.iterations.bestScore = bestScore;
        if (depth == 6) return bestMove;
        return bestScore;
    }

    function clear()
    {
        context.clearRect(0, 0, canvas.width, canvas.height); 
    }

    function tick()
    {
        // console.clear();
        console.log('player, ai, moves', player.move, ai.move);
        console.log('negamax: ', window.iterations.negamax);
        console.log('floodfill: ', window.iterations.floodfill);
        console.log('evaluatePosition: ', window.iterations.evaluatePosition);
        console.log('bestScore: ', window.iterations.bestScore);

        window.iterations = {
            negamax: 0,
            floodfill: 0,
            evaluatePosition: 0,
            bestScore: [],
        };

        // player.move = negamax(player.pos(), ai.pos(), 6, -1e6, 1e6, player.move);
        player.move = maxn([player.pos(), ai.pos()], 6, 0, player.move);
        player.update();
        player.draw();

        // ai.move = negamax(ai.pos(), player.pos(), 6, -1e6, 1e6, ai.move);
        ai.move = maxn([player.pos(), ai.pos()], 6, 1, ai.move);
        ai.update();
        ai.draw();

        if (!player.alive && !ai.alive) return false;
        setTimeout(tick, 1000);
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
