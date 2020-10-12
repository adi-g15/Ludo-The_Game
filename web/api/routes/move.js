const { Router } = require('express');

const { contains } = require('underscore');
const router = Router();

let corners = {}
corners['oc'] = {  //outer_corners
    '0,6': 'L',
    '0,8': 'D',
    '6,0': 'L',
    '6,14': 'D',
    '8,0': 'U',
    '8,14': 'R',
    '14,6': 'U',
    '14,8': 'R'
};
corners['it'] = {  //outer_corners
    '9,6': 'R',
    '6,5': 'U',
    '8,9': 'D',
    '5,8': 'L'
};

const homeTurns = { // colour: [coord,direction]
    'R': ['14,7', 'U'],
    'G': ['7,0', 'L'],
    'Y': ['0,7', 'D'],
    'B': ['7,14', 'R'],
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

function validData(reqData) {
    const allowed_colours = ['R', 'G', 'Y', 'B'];
    const allowed_dirs = ['U', 'D', 'L', 'R'];
    if (!!reqData['dist'] && !!reqData['goti'] && !!reqData['goti']['col'] && !!reqData['goti']['dir'] && !!reqData['goti']['coords']) {
        if (!contains(allowed_colours, reqData['goti']['col'])
            || !contains(allowed_dirs, reqData['goti']['col'])) {
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
            'coords': str_to_pair(req.body.goti.coords), //current coords
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
    while (--dist >= 0) {
        increment_coords = [0, 0];

        turnDirection = turnAtCorner(corners['oc'], updated_coords);
        if (turnDirection) {
            currDirection = turnDirection;
            switch (currDirection) {
                case 'U': increment_coords = [-1, 0]; break;
                case 'L': increment_coords = [0, 1]; break;
                case 'R': increment_coords = [0, -1]; break;
                case 'D': increment_coords = [1, 0]; break;
            }
        } else {
            turnDirection = turnAtCorner(corners['it'], updated_coords);

            if (turnDirection) {
                currDirection = turnDirection;
                switch (currDirection) {
                    case 'U': increment_coords = [-1, 1]; break;
                    case 'L': increment_coords = [1, 1]; break;
                    case 'R': increment_coords = [-1, -1]; break;
                    case 'D': increment_coords = [1, -1]; break;
                }
            } else {
                if (updated_coords == homeTurns[reqData['goti']['col']][0])
                    currDirection = homeTurns[reqData['goti']['col']][1];

                switch (currDirection) {
                    case 'U': increment_coords = [-1, 0]; break;
                    case 'L': increment_coords = [0, 1]; break;
                    case 'R': increment_coords = [0, -1]; break;
                    case 'D': increment_coords = [1, 0]; break;
                }
            }
        }

        updated_coords[0] += increment_coords[0];
        updated_coords[1] += increment_coords[1];

        if (isHomeEnd(updated_coords)) return console.log({ 'bool': false });

    }
    res.send({
        'bool': true,
        'move': {
            'coord': updated_coords,
            'dir': currDirection,
            'profit': null,
        }
    })

});

module.exports = router;
