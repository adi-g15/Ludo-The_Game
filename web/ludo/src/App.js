import React, { useState, useEffect } from 'react';
import './App.css';
import Dice from './Dice'
import Table from './Table';
import Board from './Board';

function App() {
	const turns = [ 'Green', 'Yellow', 'Blue', 'Red' ]
	const colors = [ "#66bb6a", "#fff176", "#29b6f6", "#e53935" ]
	const [die, setDie] = useState(1)
	const [info, setInfo] = useState("")
	const [positions, setPositions] = useState([ [], [], [], [] ])
	const [turn, setTurn] = useState(0)
	const [board, setBoard] = useState({})
	const [choice, setChoice] = useState(true)
	useEffect(() => {
		setBoard({
			"6-0": [], "6-1": [], "6-2": [], "6-3": [], "6-4": [], "6-5": [],
			"6-9": [], "6-10": [], "6-11": [], "6-12": [], "6-13": [], "6-14": [],
			"7-0": [], "7-1": [], "7-2": [], "7-3": [], "7-4": [], "7-5": [],
			"7-9": [], "7-10": [], "7-11": [], "7-12": [], "7-13": [], "7-14": [],
			"8-0": [], "8-1": [], "8-2": [], "8-3": [], "8-4": [], "8-5": [],
			"8-9": [], "8-10": [], "8-11": [], "8-12": [], "8-13": [], "8-14": [],
			"0-6": [], "1-6": [], "2-6": [], "3-6": [], "4-6": [], "5-6": [],
			"9-6": [], "10-6": [], "11-6": [], "12-6": [], "13-6": [], "14-6": [],
			"0-7": [], "1-7": [], "2-7": [], "3-7": [], "4-7": [], "5-7": [],
			"9-7": [], "10-7": [], "11-7": [], "12-7": [], "13-7": [], "14-7": [],
			"0-8": [], "1-8": [], "2-8": [], "3-8": [], "4-8": [], "5-8": [],
			"9-8": [], "10-8": [], "11-8": [], "12-8": [], "13-8": [], "14-8": [],
		})
		setPositions(position => position.map(value => value = Array(4).fill(false)))
	}, [])
	const killCheck = (position, newboard, updated_place) => {
		newboard[updated_place] = newboard[updated_place].reduce((result, current) => {
			if(current === turn) {
				result.push(current)
			}
			else {
				for(let i=0;i<4;++i) {
					if(position[current][i] && position[current][i] === updated_place) {
						position[current][i] = false
					}
				}
			}
			return result
		}, [])
	}
	const move = (x, y) => {
		setInfo("")
		fetch('https://adig15.herokuapp.com/ludo/move/goti', {
			method: 'post',
			headers: {
				'Content-Type': 'application/json'
			},
			body: JSON.stringify({
				"goti": {
					"col": ["G", "Y", "B", "R"][turn],
					"coords": [x, y]
				},
				"dist": die
			})
		})
		.then(res =>  res.json())
		.then(response => {
			if(response.bool) {
				const current_place = x + '-' + y
				const updated_place = response.move.coord[0] + '-' + response.move.coord[1]
				let newboard = { ...board }
				let position = [ ...positions ]
				newboard[current_place].splice(newboard[current_place].indexOf(turn), 1)
				if({ '7-6': 1, '6-7': 2, '7-8': 3, '8-7': 4 }[updated_place] === turn) {
					position[turn][position[turn].indexOf(current_place)] = true
					if(positions[turn].reduce((total, curr) => total + (curr === true), 0) === 4) {
						alert(turns[turn] + "won the Game! Starting a new game...")
						window.location.reload()
					}
				}
				else {
					position[turn][position[turn].indexOf(current_place)] = updated_place
					killCheck(position, newboard, updated_place)
					newboard[updated_place].push(turn)
				}
				setPositions(position)
				setBoard(newboard)
				if(die < 6)
					setTurn(turn => (turn+1)%4)
				return true
			}
			setInfo("Invalid move, select another Goti")
			return false
		})
	}
	const rollDice = () => {
		if(choice) {
			setDie(0)
			const dice = [...document.querySelectorAll(".die-list")]
			dice.forEach(die => {
				toggleClasses(die);
				fetch("https://adig15.herokuapp.com/ludo/roll")
				.then(res =>  res.json())
				.then(response => {
					const roll = response.roll[0]
					// const roll = 6
					setDie(roll)
					die.dataset.roll = roll
					const newplace = [ '6-1', '1-8', '8-13', '13-6' ]
					let newboard = { ...board }
					if(roll === 6) {
						let position = [ ...positions ]
						if((position[turn].reduce((total, curr) => curr === false ? total+1 : total, 0) === 4) || 
						((position[turn].reduce((total, curr) => curr === false ? total+1 : total, 0) > 0) && 
						(window.confirm("Press 'Ok' to unlock a new goti, or Press 'Cancel' to move a previous goti")))) {
							setInfo(turns[turn] + " unlocked a goti!")
							let i = 0
							while(position[turn][i++] !== false);
							position[turn][--i] = newplace[turn]
							killCheck(position, newboard, newplace[turn])
							newboard[newplace[turn]].push(turn)
							setBoard(newboard)
						}
						else {
							setChoice(false)
						}
						setPositions(position)
					}
					else {
						if(positions[turn].reduce((total, curr) => curr === false ? total+1 : total, 0) < 4)
							setChoice(false)
						else
							setTurn(turn => (turn+1)%4)
					}
				})
			})
		}
	}
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
					<h1 className="info" style={{ color: colors[turn], textShadow: "0 0 4px orangered" }}>{info}</h1>
				</div>
			</div>
		</div>
	)
}

function toggleClasses(die) {
	die.classList.toggle("odd-roll");
	die.classList.toggle("even-roll");
}

export default App;

// Board Credits: https://codepen.io/thewayur/pen/WNNXJMO
