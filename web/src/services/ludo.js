// implementation from underscore
function random(min, max) {
  if (max == null) {
    max = min;
    min = 0;
  }
  return min + Math.floor(Math.random() * (max - min + 1));
}

const corners = {
  oc: {
    // outer_corners
    "0,6": "R",
    "0,8": "D",
    "6,0": "R",
    "6,14": "D",
    "8,0": "U",
    "8,14": "L",
    "14,6": "U",
    "14,8": "L",
  },
  it: {
    // inner_turns
    "9,6": "L",
    "6,5": "U",
    "8,9": "D",
    "5,8": "R",
  },
};

const homeTurns = {
  // colour: [coord,direction]
  R: ["14,7", "U"],
  G: ["7,0", "R"],
  Y: ["0,7", "D"],
  B: ["7,14", "L"],
};

function isHomeEnd(coord) {
  if (Array.isArray(coord)) coord = coord.join();

  const home_ends = ["8,7", "6,7", "7,6", "7,8"];
  if (home_ends.includes(coord)) return true;
  else return false;
}

function turnAtCorner(corners_vec, coord) {
  // returns `null` to signify to direction
  return corners_vec[coord] || null;
}

function getDirection(coords) {
  if (!coords || typeof coords[0] !== "number" || typeof coords[0] !== "number")
    return null;

  if (coords[1] > 5 && coords[1] < 9) {
    if (coords[0] === 0) return "R";
    else if (coords[0] === 14) return "L";

    return coords[1] === 6
      ? "U"
      : coords[1] === 8
      ? "D"
      : coords[0] < 6
      ? "D"
      : "U"; // returning null for home path, since that is retrieved by homeTurns
  } else if (coords[0] > 5 && coords[0] < 9) {
    if (coords[1] === 0) return "U";
    else if (coords[1] === 14) return "D";

    return coords[0] === 6
      ? "R"
      : coords[0] === 8
      ? "L"
      : coords[1] < 6
      ? "R"
      : "L"; // returning null for home path, since that is retrieved by homeTurns
  }
}

function validData(colour, coords, dist) {
  const allowed_colours = ["R", "G", "Y", "B"];

  // also verifies if coords is a 2 dimensional array (expected 2D array) or not
  if (!!colour && Array.isArray(coords) && Array.isArray(coords[0]) && !!dist) {
    return allowed_colours.includes(colour);
  } else return false;
}

// 6,1, 1, G
function mover(colour, coord, dist) {
  let increment_coords = [0, 0];
  let turnDirection = null;
  const updated_coords = coord;
  let currDirection = getDirection(updated_coords);
  dist = parseInt(dist); // if it's string, convert to number

  if (!currDirection || !dist || typeof colour !== "string") return [false];

  if (dist === 0) return [false];
  while (dist-- > 0) {
    increment_coords = [0, 0];

    turnDirection = turnAtCorner(corners.oc, updated_coords);
    if (turnDirection) {
      currDirection = turnDirection;
      switch (currDirection) {
        case "U":
          increment_coords = [-1, 0];
          break;
        case "L":
          increment_coords = [0, -1];
          break;
        case "R":
          increment_coords = [0, 1];
          break;
        case "D":
          increment_coords = [1, 0];
          break;
      }
    } else {
      turnDirection = turnAtCorner(corners.it, updated_coords);

      if (turnDirection) {
        currDirection = turnDirection;
        switch (currDirection) {
          case "U":
            increment_coords = [-1, 1];
            break;
          case "L":
            increment_coords = [-1, -1];
            break;
          case "R":
            increment_coords = [1, 1];
            break;
          case "D":
            increment_coords = [1, -1];
            break;
        }
      } else {
        if (updated_coords == homeTurns[colour][0]) {
          currDirection = homeTurns[colour][1];
        }

        switch (currDirection) {
          case "U":
            increment_coords = [-1, 0];
            break;
          case "L":
            increment_coords = [0, -1];
            break;
          case "R":
            increment_coords = [0, 1];
            break;
          case "D":
            increment_coords = [1, 0];
            break;
        }
      }
    }

    updated_coords[0] += increment_coords[0];
    updated_coords[1] += increment_coords[1];

    if (isHomeEnd(updated_coords) && dist > 0) return [false];
  }

  return [
    true, // possibility
    updated_coords, // final coords
  ];
}

export function moveGoti(colour, coords, dist) {
  if (Array.isArray(coords) && !Array.isArray(coords[0])) {
    // ie. is a 1D array (ie. a pair, array of length 2)
    coords = [coords];
  }

  if (!validData(colour, coords, dist)) {
    // return new Error("Input Not Valid");
    console.error("Input not valid: ", colour, coords, dist);
    return [false];
  }

  const bools = []; // an array of bools
  const finalCoords = []; // array of final coordinates

  let possibility, finalCoord;
  coords.forEach((coord) => {
    [possibility, finalCoord] = mover(colour, coord, dist);
    bools.push(possibility);
    finalCoords.push(finalCoord);
  });

  console.debug(`Input: ${coords}, moved ${dist} units, final result: `, {
    isPossible: bools,
    finalCoords,
  });

  return {
    isPossible: bools,
    finalCoords: finalCoords,
  };
}

export function rollDiceOnce() {
  return random(1, 6);
}

export function multipleRoll() {
  let n = random(1, 6);
  const arr = [];
  arr.push(n);

  let six_count = 0;
  while (n === 6) {
    n = random(1, 6);

    six_count++; // to remove three consecutive sixes
    if (six_count === 3) {
      arr.pop();
      arr.pop(); // previous two were 6, and this turn 6 won't be pushed so only two 6 in array

      six_count = 0;
      continue;
    }

    arr.push(n);
  }

  return arr;
}

moveGoti("G", [6, 1], 1);
moveGoti("R", [13, 6], 5);
