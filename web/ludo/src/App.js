import React, { useState, useEffect } from 'react';
import './App.css';

function App() {
	const turns = [ 'Green', 'Yellow', 'Blue', 'Red' ]
	const colors = [ "#66bb6a", "#fff176", "#29b6f6", "#e53935" ]
	const [players, setPlayers] = useState([ 4, 4, 4, 4 ])
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
	}, [])
	const move = (x, y) => {
		// fetch('https://adig15.herokuapp.com/ludo/move/goti', {
		// 	method: 'post',
		// 	body: JSON.stringify({
		// 		"goti": {
		// 			"col": ["G", "Y", "B", "R"][turn],
		// 			"dir": "U",
		// 			"coords": [6,9]
		// 		},
		// 		"dist": 3
		// 	})
		// })
		// .then(res =>  res.json(value))
		// .then(response => {

		// })
	}
	const rollDice = () => {
		const dice = [...document.querySelectorAll(".die-list")]
		dice.forEach(die => {
			toggleClasses(die);
			fetch("https://adig15.herokuapp.com/ludo/roll")
			.then(res =>  res.json())
			.then(response => {
				const roll = response.roll[0]
				// const roll = 6
				die.dataset.roll = roll
				const ply = [ ...players ]
				const newplace = [ '6-1', '1-8', '8-13', '13-6' ]
				let newboard = { ...board }
				if(roll === 6) {
					if(players[turn] === 4) {
						ply[turn] -= 1
						setPlayers(ply)
						alert(turns[turn] + " unlocked a goti!")
						newboard[newplace[turn]].push(turn)
						setBoard(newboard)
					}
					else if(players[turn] > 0) {
						if(window.confirm("Press 'Ok' to unlock a new goti, or Press 'Cancel' to move a previous goti")) {
							ply[turn] -= 1
							setPlayers(ply)
							alert("You unlocked a goti!")
							newboard[newplace[turn]].push(turn)
							setBoard(newboard)
						}
					}
				}
				else {
					// setChoice(false)
					setTurn(turn => (turn+1)%4)
				}
			})
		})
	}
	return (
		<div className="App">
			<div className="game">
				<div className="house green">
					<div className="box">
						<div className="square square-one green">
							{players[0] > 0 ? <Goti color={colors[0]} /> : null}
						</div>
						<div className="square square-two green">
							{players[0] > 1 ? <Goti color={colors[0]} /> : null}
						</div>
						<div className="square square-three green">
							{players[0] > 2 ? <Goti color={colors[0]} /> : null}
						</div>
						<div className="square square-four green">
							{players[0] > 3 ? <Goti color={colors[0]} /> : null}
						</div>
					</div>
				</div>
				<div className="house yellow" style={{right: 0}}>
					<div className="box">
						<div className="square square-one yellow">
							{players[1] > 0 ? <Goti color={colors[1]} /> : null}
						</div>
						<div className="square square-two yellow">
							{players[1] > 1 ? <Goti color={colors[1]} /> : null}
						</div>
						<div className="square square-three yellow">
							{players[1] > 2 ? <Goti color={colors[1]} /> : null}
						</div>
						<div className="square square-four yellow">
							{players[1] > 3 ? <Goti color={colors[1]} /> : null}
						</div>
					</div>
				</div>
				<div className="house red" style={{bottom: 0}}>
					<div className="box">
						<div className="square square-one red">
							{players[2] > 0 ? <Goti color={colors[3]} /> : null}
						</div>
						<div className="square square-two red">
							{players[2] > 1 ? <Goti color={colors[3]} /> : null}
						</div>
						<div className="square square-three red">
							{players[2] > 2 ? <Goti color={colors[3]} /> : null}
						</div>
						<div className="square square-four red">
							{players[2] > 3 ? <Goti color={colors[3]} /> : null}
						</div>
					</div>
				</div>
				<div className="house blue" style={{bottom: 0, right: 0}}>
					<div className="box">
						<div className="square square-one blue">
							{players[3] > 0 ? <Goti color={colors[2]} /> : null}
						</div>
						<div className="square square-two blue">
							{players[3] > 1 ? <Goti color={colors[2]} /> : null}
						</div>
						<div className="square square-three blue">
							{players[3] > 2 ? <Goti color={colors[2]} /> : null}
						</div>
						<div className="square square-four blue">
							{players[3] > 3 ? <Goti color={colors[2]} /> : null}
						</div>
					</div>
				</div>
				<div className="home" />
				<div className="cells" style={{top: '40%'}}>
					{ board['6-0'] && board['6-0'].length ? <Goti move={move} place="6-0" glow={board['6-0'].includes(turn)} color={colors[board['6-0']]} /> : null }
				</div>
				<div className="cells g-start" style={{top: '40%', left: '6.66%'}} />
				<div className="cells" style={{top: '40%', left: '6.66%'}} >
					{ board['6-1'] && board['6-1'].length ? <Goti move={move} place="6-1" glow={board['6-1'].includes(turn)} color={colors[board['6-1']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', left: '13.32%'}}>
					{ board['6-2'] && board['6-2'].length ? <Goti move={move} place="6-2" glow={board['6-2'].includes(turn)} color={colors[board['6-2']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', left: '19.98%'}}>
					{ board['6-3'] && board['6-3'].length ? <Goti move={move} place="6-3" glow={board['6-3'].includes(turn)} color={colors[board['6-3']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', left: '26.64%'}}>
					{ board['6-4'] && board['6-4'].length ? <Goti move={move} place="6-4" glow={board['6-4'].includes(turn)} color={colors[board['6-4']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', left: '33.3%'}}>
					{ board['6-5'] && board['6-5'].length ? <Goti move={move} place="6-5" glow={board['6-5'].includes(turn)} color={colors[board['6-5']]} /> : null }
				</div>
				<div className="cells" style={{top: 0, left: '40%'}} />
				<div className="cells" style={{top: '6.66%', left: '40%'}} />
				<div className="cells safe" style={{top: '13.32%', left: '40%'}} />
				<div className="cells" style={{top: '19.98%', left: '40%'}} />
				<div className="cells" style={{top: '26.64%', left: '40%'}} />
				<div className="cells" style={{top: '33.3%', left: '40%'}} />
				<div className="cells" style={{top: 0, left: '46.66%'}} />
				<div className="cells yellow" style={{top: '6.66%', left: '46.66%'}} />
				<div className="cells yellow" style={{top: '13.32%', left: '46.66%'}} />
				<div className="cells yellow" style={{top: '19.98%', left: '46.66%'}} />
				<div className="cells yellow" style={{top: '26.64%', left: '46.66%'}} />
				<div className="cells yellow" style={{top: '33.3%', left: '46.66%'}} />
				<div className="cells" style={{top: 0, left: '53.32%'}}>
					{ board['0-8'] && board['0-8'].length ? <Goti move={move} place="0-8" glow={board['0-8'].includes(turn)} color={colors[board['0-8']]} /> : null }
				</div>
				<div className="cells y-start" style={{top: '6.66%', left: '53.32%'}} />
				<div className="cells" style={{top: '6.66%', left: '53.32%'}}>
					{ board['1-8'] && board['1-8'].length ? <Goti move={move} place="1-8" glow={board['1-8'].includes(turn)} color={colors[board['1-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '13.32%', left: '53.32%'}}>
					{ board['2-8'] && board['2-8'].length ? <Goti move={move} place="2-8" glow={board['2-8'].includes(turn)} color={colors[board['2-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '19.98%', left: '53.32%'}}>
					{ board['3-8'] && board['3-8'].length ? <Goti move={move} place="3-8" glow={board['3-8'].includes(turn)} color={colors[board['3-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '26.64%', left: '53.32%'}}>
					{ board['4-8'] && board['4-8'].length ? <Goti move={move} place="4-8" glow={board['4-8'].includes(turn)} color={colors[board['4-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '33.3%', left: '53.32%'}}>
					{ board['5-8'] && board['5-8'].length ? <Goti move={move} place="5-8" glow={board['5-8'].includes(turn)} color={colors[board['5-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', right: '33.3%'}} />
				<div className="cells" style={{top: '40%', right: '26.64%'}} />
				<div className="cells" style={{top: '40%', right: '19.98%'}} />
				<div className="cells safe" style={{top: '40%', right: '13.32%'}} />
				<div className="cells" style={{top: '40%', right: '6.66%'}} />
				<div className="cells" style={{top: '40%', right: 0}} />
				<div className="cells blue" style={{top: '46.66%', right: '33.3%'}} />
				<div className="cells blue" style={{top: '46.66%', right: '26.64%'}} />
				<div className="cells blue" style={{top: '46.66%', right: '19.98%'}} />
				<div className="cells blue" style={{top: '46.66%', right: '13.32%'}} />
				<div className="cells blue" style={{top: '46.66%', right: '6.66%'}} />
				<div className="cells" style={{top: '46.66%', right: 0}} />
				<div className="cells" style={{top: '53.32%', right: '33.3%'}} />
				<div className="cells" style={{top: '53.32%', right: '26.64%'}} />
				<div className="cells" style={{top: '53.32%', right: '19.98%'}} />
				<div className="cells" style={{top: '53.32%', right: '13.32%'}}>
					{ board['8-14'] && board['8-14'].length ? <Goti move={move} place="8-14" glow={board['8-14'].includes(turn)} color={colors[board['8-14']]} /> : null }
				</div>
				<div className="cells b-start" style={{top: '53.32%', right: '6.66%'}} />
				<div className="cells" style={{top: '53.32%', right: '6.66%'}}>
					{ board['8-13'] && board['8-13'].length ? <Goti move={move} place="8-13" glow={board['8-13'].includes(turn)} color={colors[board['8-13']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', right: 0}}>
					{ board['8-12'] && board['8-12'].length ? <Goti move={move} place="8-12" glow={board['8-12'].includes(turn)} color={colors[board['8-12']]} /> : null }
				</div>
				<div className="cells" style={{bottom: 0, left: '53.32%'}}>
					{ board['8-11'] && board['8-11'].length ? <Goti move={move} place="8-11" glow={board['8-11'].includes(turn)} color={colors[board['8-11']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '6.66%', left: '53.32%'}}>
					{ board['8-10'] && board['8-10'].length ? <Goti move={move} place="8-10" glow={board['8-10'].includes(turn)} color={colors[board['8-10']]} /> : null }
				</div>
				<div className="cells safe" style={{bottom: '13.32%', left: '53.32%'}}>
					{ board['8-9'] && board['8-9'].length ? <Goti move={move} place="8-9" glow={board['8-9'].includes(turn)} color={colors[board['8-9']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '19.98%', left: '53.32%'}} />
				<div className="cells" style={{bottom: '26.64%', left: '53.32%'}} />
				<div className="cells" style={{bottom: '33.3%', left: '53.32%'}} />
				<div className="cells" style={{bottom: 0, left: '46.66%'}} />
				<div className="cells red " style={{bottom: '6.66%', left: '46.66%'}} />
				<div className="cells red" style={{bottom: '13.32%', left: '46.66%'}} />
				<div className="cells red" style={{bottom: '19.98%', left: '46.66%'}} />
				<div className="cells red" style={{bottom: '26.64%', left: '46.66%'}} />
				<div className="cells red" style={{bottom: '33.3%', left: '46.66%'}} />
				<div className="cells" style={{bottom: 0, left: '40%'}}>
				{ board['14-6'] && board['14-6'].length ? <Goti move={move} place="14-6" glow={board['14-6'].includes(turn)} color={colors[board['14-6']]} /> : null }
				</div>
				<div className="cells r-start" style={{bottom: '6.66%', left: '40%'}} />
				<div className="cells" style={{bottom: '6.66%', left: '40%'}}>
					{ board['13-6'] && board['13-6'].length ? <Goti move={move} place="13-6" glow={board['13-6'].includes(turn)} color={colors[board['13-6']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '13.32%', left: '40%'}}>
					{ board['12-6'] && board['12-6'].length ? <Goti move={move} place="12-6" glow={board['12-6'].includes(turn)} color={colors[board['12-6']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '19.98%', left: '40%'}}>
					{ board['11-6'] && board['11-6'].length ? <Goti move={move} place="11-6" glow={board['11-6'].includes(turn)} color={colors[board['11-6']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '26.64%', left: '40%'}}>
					{ board['10-6'] && board['10-6'].length ? <Goti move={move} place="10-6" glow={board['10-6'].includes(turn)} color={colors[board['10-6']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '33.3%', left: '40%'}}>
					{ board['9-6'] && board['9-6'].length ? <Goti move={move} place="9-6" glow={board['9-6'].includes(turn)} color={colors[board['9-6']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', left: '33.3%'}} />
				<div className="cells" style={{top: '53.32%', left: '26.64%'}} />
				<div className="cells" style={{top: '53.32%', left: '19.98%'}} />
				<div className="cells safe" style={{top: '53.32%', left: '13.32%'}} />
				<div className="cells" style={{top: '53.32%', left: '6.66%'}} />
				<div className="cells" style={{top: '53.32%', left: 0}} />
				<div className="cells green" style={{top: '46.66%', left: '33.3%'}} />
				<div className="cells green" style={{top: '46.66%', left: '26.64%'}} />
				<div className="cells green" style={{top: '46.66%', left: '19.98%'}} />
				<div className="cells green" style={{top: '46.66%', left: '13.32%'}} />
				<div className="cells green" style={{top: '46.66%', left: '6.66%'}} />
				<div className="cells" style={{top: '46.66%', left: 0}} />
			</div>
			<div className="panel">
				<div className="info">
					<table style={{ margin: "0 auto"}}>
						<thead>
							<tr>
								<th>Player</th>
								<th>Gotis Locked</th>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td>Green</td>
								<td>{players[0]}</td>
							</tr>
							<tr>
								<td>Yellow</td>
								<td>{players[1]}</td>
							</tr>
							
							<tr>
								<td>Blue</td>
								<td>{players[2]}</td>
							</tr>
							<tr>
								<td>Red</td>
								<td>{players[3]}</td>
							</tr>
						</tbody>
					</table>
				</div>
				<div className="dice">
					<Turn turn={turns[turn]} color={colors[turn]} choice={choice} />
					<ol className="die-list even-roll" data-roll="1" onClick={() => { rollDice(turn) }}>
						<li className="die-item" data-side="1">
						<span className="dot"></span>
						</li>
						<li className="die-item" data-side="2">
						<span className="dot"></span>
						<span className="dot"></span>
						</li>
						<li className="die-item" data-side="3">
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						</li>
						<li className="die-item" data-side="4">
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						</li>
						<li className="die-item" data-side="5">
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						</li>
						<li className="die-item" data-side="6">
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						<span className="dot"></span>
						</li>
					</ol>
				</div>
			</div>
		</div>
	)
}

function Goti(props) {
	if(props.place) {
		const coords = props.place.split("-")
		return <figure 
			className={"circle " + (props.glow ? "glow" : null)}
			style={{ background: "radial-gradient(circle at 33% 33%, " + props.color + ", #000)" }}
			onClick={props.glow ? props.move(coords[0], coords[1]) : null}
		/>
	}
	return <figure 
		className={"circle " + (props.glow ? "glow" : null)}
		style={{ background: "radial-gradient(circle at 33% 33%, " + props.color + ", #000)" }}
	/>
}

function Turn(props) {
	return (
		<h1>
			{props.choice ? <span>Its <span style={{ color: props.color }}>{props.turn}'s</span> turn</span> : "Choose a Goti to move" }
		</h1>
	)
}

function toggleClasses(die) {
	die.classList.toggle("odd-roll");
	die.classList.toggle("even-roll");
}

export default App;

// Board Credits: https://codepen.io/thewayur/pen/WNNXJMO
