const { Router } = require('express');

const { contains } = require('underscore');
const router = Router();

let corners = {}
corners['oc'] = {  //outer_corners
    '0,6': 'R',
    '0,8': 'D',
    '6,0': 'R',
    '6,14': 'D',
    '8,0': 'U',
    '8,14': 'L',
    '14,6': 'U',
    '14,8': 'L'
};
corners['it'] = {  //outer_corners
    '9,6': 'L',
    '6,5': 'U',
    '8,9': 'D',
    '5,8': 'R'
};

const homeTurns = { // colour: [coord,direction]
    'R': ['14,7', 'U'],
    'G': ['7,0', 'R'],
    'Y': ['0,7', 'D'],
    'B': ['7,14', 'L'],
};

function isHomeEnd(coords) {
    home_ends = ['8,7', '6,7', '7,6', '7,8'];
    if (home_ends.find((end) => end == coords)) return true;
    else return false;
}

function turnAtCorner(corners_vec, coord) {   // returns `null` to signify to direction
    return corners_vec[coord] || null;
}

function str_to_pair(str) {
    if (!str || !str[0] || !str[1]) return null;
    if (typeof (str[0]) === 'number' && typeof (str[1]) === 'number') return str;
    if (str[0] === '[') {
        str.pop(); str.shift();
    }

    let temp = str.split(',');
    if (temp.length > 2) return null;
    return [Number(temp[0]), Number(temp[1])];
}

function getDirection(coords){
    if( !coords || typeof(coords[0]) !== 'number' ||  typeof(coords[0]) !== 'number' ) return null;

    if( coords[1] > 5 && coords[1] < 9 ){
        if(coords[0] == 0)   return 'R';
        else if(coords[0] == 14)   return 'L';

        return coords[1] === 6 ? 'U' : (coords[1] === 8 ? 'D' : coords[0]<6 ? 'D': 'U');  //returning null for home path, since that is retrieved by homeTurns
    } else if (coords[0] > 5 && coords[0] < 9) {
        if (coords[1] == 0) return 'U';
        else if (coords[1] == 14) return 'D';

        return coords[0] === 6 ? 'R' : (coords[0] === 8 ? 'L' : coords[1]<6 ? 'R': 'L');  //returning null for home path, since that is retrieved by homeTurns
    }
}

function validData(reqData) {
    const allowed_colours = ['R', 'G', 'Y', 'B'];
    const allowed_dirs = ['U', 'D', 'L', 'R'];
    if (!reqData['goti']['dir']) {
        reqData['goti']['dir'] = getDirection(reqData['goti']['coords']);
    }

    if (!!reqData['dist'] && !!reqData['goti'] && !!reqData['goti']['col'] && !!reqData['goti']['dir'] && !!reqData['goti']['coords']) {
        if (!contains(allowed_colours, reqData['goti']['col'])
            || !contains(allowed_dirs, reqData['goti']['dir'])) {
            return false;
        }
        else return true;
    } else return false;
}

router.post('/goti', (req, res) => {
    let reqData = {
        'goti': {
            'col': req.body.goti.col,   //colour
            'dir': req.body.goti.dir,   //direction
            'coords': req.body.goti.coords, //current coords
        },
        'dist': Number(req.body.dist),
    }

    if (!validData(reqData)) {
        return res.status(400).send({ error: 'Input Not Valid', inputReceived: reqData });
    }

    let dist = reqData['dist'];

    if (dist === 0)
        return res.send({ 'bool': false });

    let increment_coords = [0, 0];
    let updated_coords = reqData['goti']['coords'];
    let turnDirection = 'NO_TURN';
    let currDirection = reqData['goti']['dir']
    while (dist-- > 0) {
        increment_coords = [0, 0];

        turnDirection = turnAtCorner(corners['oc'], updated_coords);
        if (turnDirection) {
            currDirection = turnDirection;
            switch (currDirection) {
                case 'U': increment_coords = [-1, 0]; break;
                case 'L': increment_coords = [0, -1]; break;
                case 'R': increment_coords = [0, 1]; break;
                case 'D': increment_coords = [1, 0]; break;
            }
        } else {
            turnDirection = turnAtCorner(corners['it'], updated_coords);

            if (turnDirection) {
                currDirection = turnDirection;
                switch (currDirection) {
                    case 'U': increment_coords = [-1, 1]; break;
                    case 'L': increment_coords = [-1, -1]; break;
                    case 'R': increment_coords = [1, 1]; break;
                    case 'D': increment_coords = [1, -1]; break;
                }
            } else {
                if (updated_coords == homeTurns[reqData['goti']['col']][0])
                    currDirection = homeTurns[reqData['goti']['col']][1];

                switch (currDirection) {
                    case 'U': increment_coords = [-1, 0]; break;
                    case 'L': increment_coords = [0, -1]; break;
                    case 'R': increment_coords = [0, 1]; break;
                    case 'D': increment_coords = [1, 0]; break;
                }
            }
        }

        updated_coords[0] += increment_coords[0];
        updated_coords[1] += increment_coords[1];

        if (isHomeEnd(updated_coords) && dist>0) return res.send({ 'bool': false });

    }

    res.send({
        'bool': true,
        'move': {
            'coord': updated_coords,
            'dir': currDirection,
        }
    })

});

module.exports = router;
