import React, { useState, useEffect } from 'react';
import './App.css';
import Dice from './components/Dice';
import Table from './components/Table';
import Board from './components/Board';
import { rollDice, moveGoti } from "./services/ludo"

function App() {
	const turns = ['Green', 'Yellow', 'Blue', 'Red'];
	const colors = ['#66bb6a', '#fff176', '#29b6f6', '#e53935'];
	const [die, setDie] = useState(1);													// Stores current value of die
	const [info, setInfo] = useState('');												// Stores "info" text(if any) eg. Unlocked Goti
	const [positions, setPositions] = useState([[], [], [], []]);						// Stores positions of Gotis of all colors (4x4 array)
	const [turn, setTurn] = useState(0);													// Green-0, Yellow-1, Blue-2, Red-3 (current turn)
	const [board, setBoard] = useState({});												// Store content of each block on board
	const [choice, setChoice] = useState(true);											// true-> roll die, false-> move goti
	useEffect(() => {
		setBoard({
			'6-0': [], '6-1': [], '6-2': [], '6-3': [], '6-4': [], '6-5': [],			// Key:value pairs
			'6-9': [], '6-10': [], '6-11': [], '6-12': [], '6-13': [], '6-14': [],		// Key - 'Row-Column' format of every valid place
			'7-0': [], '7-1': [], '7-2': [], '7-3': [], '7-4': [], '7-5': [],			// on the board
			'7-9': [], '7-10': [], '7-11': [], '7-12': [], '7-13': [], '7-14': [],		// Value - Array (possible empty) containing values
			'8-0': [], '8-1': [], '8-2': [], '8-3': [], '8-4': [], '8-5': [],			// according to 'turn' numbering that gives count of
			'8-9': [], '8-10': [], '8-11': [], '8-12': [], '8-13': [], '8-14': [],		// gotis present in that box.
			'0-6': [], '1-6': [], '2-6': [], '3-6': [], '4-6': [], '5-6': [],			// Will either be empty or having 1-4 elements having
			'9-6': [], '10-6': [], '11-6': [], '12-6': [], '13-6': [], '14-6': [],		// identical value, except in case of safe blocks.
			'0-7': [], '1-7': [], '2-7': [], '3-7': [], '4-7': [], '5-7': [],			// Safe blocks may contain different values since
			'9-7': [], '10-7': [], '11-7': [], '12-7': [], '13-7': [], '14-7': [],		// different color gotis can survive simultaneously
			'0-8': [], '1-8': [], '2-8': [], '3-8': [], '4-8': [], '5-8': [],			// at those places.
			'9-8': [], '10-8': [], '11-8': [], '12-8': [], '13-8': [], '14-8': [],
		});
		// eslint-disable-next-line no-unused-vars
		setPositions(position => position.map(value => value = Array(4).fill(false)));	// Initialize all as false (false = gotis at home)
	}, []);
	const move = (current_place) => {
		setInfo('');
		const coords = current_place.split('-');											// Coordinates of current place taken from 'Goti'

		try{
			const {isPossible, finalCoords} = moveGoti(
				['G', 'Y', 'B', 'R'][turn],	//colour
				[parseInt(coords[0]), parseInt(coords[1])],	//coords
				die	// distance_to_move
			);

			if (isPossible[0]) {
				const updated_place = finalCoords[0][0] + '-' + finalCoords[0][1];	// updated coordinates from response
				let newboard = { ...board };													// Copy of boards & positions to make changes
				let position = [...positions];												// before setting the state
				newboard[current_place].splice(newboard[current_place].indexOf(turn), 1);	// Goti removed from current place
				if ({ '7-6': 0, '6-7': 1, '7-8': 2, '8-7': 3 }[updated_place] === turn) {	// Check if updated position is the final position
					position[turn][position[turn].indexOf(current_place)] = true;			// of current turn.If yes, position of current goti
					if (positions[turn].reduce((total, curr) => total + (curr === true), 0) === 4) {	// is set 'true', meaning reached end.
						alert(turns[turn] + ' won the Game! Starting a new game...');		// If the count of 'true' in position of current
						window.location.reload();											// turn = 4, means current player won the game.
					}
				}
				else {
					position[turn][position[turn].indexOf(current_place)] = updated_place;	// Update new positions to updatd position of goti
					if (!['8-2', '2-6', '6-12', '12-8'].includes(updated_place)) {			// If updated position is not a safe block, check
						newboard[updated_place] = newboard[updated_place].reduce((result, current) => {	// if the updated place has any gotis.
							if (current === turn) {											// If there are gotis of same color, keep adding
								result.push(current);										// them to an array to finally update the whole
							}																// block with all those gotis.
							else {
								for (let i = 0; i < 4; ++i) {										// If there are gotis of another color, kill them
									if (position[current][i] && position[current][i] === updated_place) {	// by not adding them to result
										position[current][i] = false;						// array and setting their position as false(home)
									}
								}
							}
							return result;													// return result array to update board block
						}, []);
					}
					newboard[updated_place].push(turn);										// Finally add current moved goti to the board
				}
				setPositions(position);														// Setstate of positions and board arrays
				setBoard(newboard);
				if (die < 6)
					setTurn(turn => (turn + 1) % 4);												// If die value < 6, pass turn to next player
				setChoice(true);
			}
			else {
				setInfo('Invalid move, select another Goti');								// If server responds 'false' for move, give error
			}
		} catch(err) {
			alert(typeof err === "string" ? err : "Move nahi kiya ja sakta... Try again...");
			console.error(err);
		}

	};
	async function checkPossible(roll) {
	let flag = true;																		// Flag checks if avalable gotis are at end part
	let locked = false;																		// Locked checks if there are any locked gotis
		let coords = [];
		// eslint-disable-next-line no-unused-vars
		return new Promise((resolve, reject) => {
			positions[turn].forEach(value => {
				if (typeof (value) === 'string') {
					const coord = value.split('-');											// If gotis are not at the last(colored) part
					coords.push([parseInt(coord[0]), parseInt(coord[1])]);					// of the board, any move is possible and hence
					if (coord[turn % 2] !== '7' || (coord[turn % 2] === '7' && ['0', '14'].includes(coord[(turn % 2) ^ 1]))) {	// set flag=false
						flag = false;
					}
				}
				else if (value === false) {													// If any gotis are locked, set locked=true
					locked = true;
				}
			});
			if (flag) {
				/*
					If all unlocked gotis are at the last part check if move is actually possible according to their current positions and die value.
					If yes, ask for the move
					If no, give turn to next player if die < 6.
					If no & die = 6, Unlock a goti if there are locked gotis; and give a re-chance to roll the die.
				*/
				try {
					const { isPossible } = moveGoti(	// we only need the isPossible array to check if any possible moves are left
						['G', 'Y', 'B', 'R'][turn],
						coords,	// starting coordinates to check
						roll	// distance to move from each coord
					);

					if (isPossible.reduce((total, current) => total + (current), 0) === 0) {
						if (locked)												// Return 2 => Move always possible
							resolve(1);											//
						else													// Return 1 => Move possible only if die = 6
							resolve(0);											// (By unlocking locked piece)
					}															//
					else {														// Return 0 => Move not possible
						resolve(2);
					}
				} catch (err) {
					alert(typeof err === "string" ? err : "Move nahi kiya ja sakta... Try again...");
					console.error(err);
				}
			}
			else {
				resolve(2);
			}
		});
	}
	const rollDice = () => {
		if(!choice)	return;
		// if (choice) {

		setDie(0);
		const dice = [...document.querySelectorAll('.die-list')];
		dice.forEach(die => {
			toggleClasses(die);

			const roll = rollDice();
			// const roll = 6
			setInfo('');
			setDie(roll);
			die.dataset.roll = roll;
			checkPossible(roll).then(possible => {
				if (possible) {
					const newplace = ['6-1', '1-8', '8-13', '13-6'];		// Unlock places for each color(according to turn)
					// const newplace = [ '7-1', '1-7', '7-13', '13-7' ]
					let newboard = { ...board };
					if (roll === 6) {										// If the die rolls a 6:
						let position = [...positions];
						if ((position[turn].reduce((total, curr) => total + (curr === false), 0) === position[turn].reduce((total, curr) => total - (curr === true), 4)) || possible === 1 ||
							((position[turn].reduce((total, curr) => total + (curr === false), 0) > 0) && (window.confirm('Press \'Ok\' to unlock a new goti, or Press \'Cancel\' to move a previous goti')))) {
							setInfo(turns[turn] + ' unlocked a goti!');		// Checks if there are any unlocked gotis
							let i = 0;										// If no, unlock a goti
							while (position[turn][i++] !== false);			// If yes, check if there are any locked gotis
							position[turn][--i] = newplace[turn];			// If there are, ask whether player wants to unlock a
							newboard[newplace[turn]].push(turn);				// goti or move unlocked gotis.
							setBoard(newboard);								// If there arent, just ask player to move a goti
						}
						else if (possible === 2) {
							setChoice(false);
						}
						else {
							setTurn(turn => (turn + 1) % 4);
						}
						setPositions(position);
					}
					else {
						if (positions[turn].reduce((total, curr) => total + (curr === false), 0) < 4 && possible === 2)
							setChoice(false);								// If there are any movable gotis and if move is
						else												// possible, set choice=false, i.e. ask for move
							setTurn(turn => (turn + 1) % 4);
					}
				}
				else if (roll < 6)
					setTurn(turn => (turn + 1) % 4);
				});
			});
	};
	return (
		<div className="App">
			<Board board={board} move={move} setChoice={setChoice} turn={turn} choice={choice} colors={colors} positions={positions} />
			<div className="panel">
				<div className="panel-top">
					<Table positions={positions} />
				</div>
				<div className="panel-middle">
					<Dice turn={turn} turns={turns[turn]} choice={choice} color={colors[turn]} die={die} rollDice={rollDice} />
				</div>
				<div className="panel-bottom">
					<h1 className="info" style={{ color: colors[turn], textShadow: '0 0 4px orangered' }}>{info}</h1>
				</div>
			</div>
		</div>
	);
}

function toggleClasses(die) {
	die.classList.toggle('odd-roll');
	die.classList.toggle('even-roll');
}

export default App;

// Board Credits: https://codepen.io/thewayur/pen/WNNXJMO
