import React, { useState, useEffect } from 'react';
import './App.css';
import Goti from './Goti'
import Turn from './Turn'

function App() {
	const turns = [ 'Green', 'Yellow', 'Blue', 'Red' ]
	const [die, setDie] = useState(1)
	const colors = [ "#66bb6a", "#fff176", "#29b6f6", "#e53935" ]
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
	const move = (x, y) => {
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
				let temp = { ...board }
				let position = [ ...positions ]
				position[turn][position[turn].indexOf(current_place)] = updated_place
				temp[current_place].splice(temp[current_place].indexOf(turn), 1)
				// temp[updated_place] = temp[updated_place].filter(value => { return value === turn })
				temp[updated_place] = temp[updated_place].reduce((result, current) => {
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
				setPositions(position)
				temp[updated_place].push(turn)
				setBoard(temp)
				if(die < 6)
					setTurn(turn => (turn+1)%4)
			}
			else {
				let position = [ ...positions ]
				for(let i=0;i<4;++i) {
					if(position[turn][i] === (x + '-' + y)) {
						position[turn][i] = true
						break
					}
				}
				setPositions(position)
			}
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
						if(position[turn].reduce((total, curr) => curr === false ? total+1 : total, 0) === 4) {
							alert(turns[turn] + " unlocked a goti!")
							let i = 0
							while(position[turn][i++] !== false);
							position[turn][--i] = newplace[turn]
							newboard[newplace[turn]].push(turn)
							setBoard(newboard)
						}
						else if(position[turn].reduce((total, curr) => curr === false ? total+1 : total, 0) > 0) {
							if(window.confirm("Press 'Ok' to unlock a new goti, or Press 'Cancel' to move a previous goti")) {
								alert("You unlocked a goti!")
								let i = 0
								while(position[turn][i++] !== false);
								position[turn][--i] = newplace[turn]
								newboard[newplace[turn]].push(turn)
								setBoard(newboard)
							}
							else {
								setChoice(false)
							}
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
			<div className="game">
				<div className="house green">
					<div className="box">
						<div className="square square-one green">
							{positions[0].reduce((total, curr) => curr === false ? total+1 : total, 0) > 0 ? <Goti color={colors[0]} /> : null}
						</div>
						<div className="square square-two green">
							{positions[0].reduce((total, curr) => curr === false ? total+1 : total, 0) > 1 ? <Goti color={colors[0]} /> : null}
						</div>
						<div className="square square-three green">
							{positions[0].reduce((total, curr) => curr === false ? total+1 : total, 0) > 2 ? <Goti color={colors[0]} /> : null}
						</div>
						<div className="square square-four green">
							{positions[0].reduce((total, curr) => curr === false ? total+1 : total, 0) > 3 ? <Goti color={colors[0]} /> : null}
						</div>
					</div>
				</div>
				<div className="house yellow" style={{right: 0}}>
					<div className="box">
						<div className="square square-one yellow">
							{positions[1].reduce((total, curr) => curr === false ? total+1 : total, 0) > 0 ? <Goti color={colors[1]} /> : null}
						</div>
						<div className="square square-two yellow">
							{positions[1].reduce((total, curr) => curr === false ? total+1 : total, 0) > 1 ? <Goti color={colors[1]} /> : null}
						</div>
						<div className="square square-three yellow">
							{positions[1].reduce((total, curr) => curr === false ? total+1 : total, 0) > 2 ? <Goti color={colors[1]} /> : null}
						</div>
						<div className="square square-four yellow">
							{positions[1].reduce((total, curr) => curr === false ? total+1 : total, 0) > 3 ? <Goti color={colors[1]} /> : null}
						</div>
					</div>
				</div>
				<div className="house red" style={{bottom: 0}}>
					<div className="box">
						<div className="square square-one red">
							{positions[3].reduce((total, curr) => curr === false ? total+1 : total, 0) > 0 ? <Goti color={colors[3]} /> : null}
						</div>
						<div className="square square-two red">
							{positions[3].reduce((total, curr) => curr === false ? total+1 : total, 0) > 1 ? <Goti color={colors[3]} /> : null}
						</div>
						<div className="square square-three red">
							{positions[3].reduce((total, curr) => curr === false ? total+1 : total, 0) > 2 ? <Goti color={colors[3]} /> : null}
						</div>
						<div className="square square-four red">
							{positions[3].reduce((total, curr) => curr === false ? total+1 : total, 0) > 3 ? <Goti color={colors[3]} /> : null}
						</div>
					</div>
				</div>
				<div className="house blue" style={{bottom: 0, right: 0}}>
					<div className="box">
						<div className="square square-one blue">
							{positions[2].reduce((total, curr) => curr === false ? total+1 : total, 0) > 0 ? <Goti color={colors[2]} /> : null}
						</div>
						<div className="square square-two blue">
							{positions[2].reduce((total, curr) => curr === false ? total+1 : total, 0) > 1 ? <Goti color={colors[2]} /> : null}
						</div>
						<div className="square square-three blue">
							{positions[2].reduce((total, curr) => curr === false ? total+1 : total, 0) > 2 ? <Goti color={colors[2]} /> : null}
						</div>
						<div className="square square-four blue">
							{positions[2].reduce((total, curr) => curr === false ? total+1 : total, 0) > 3 ? <Goti color={colors[2]} /> : null}
						</div>
					</div>
				</div>
				<div className="home" />
				<div className="cells" style={{top: '40%'}}>
					{ board['6-0'] && board['6-0'].length ? <Goti move={move} setChoice={setChoice} place="6-0" glow={board['6-0'].includes(turn) && !choice} color={colors[board['6-0']]} /> : null }
				</div>
				<div className="cells g-start" style={{top: '40%', left: '6.66%'}} />
				<div className="cells" style={{top: '40%', left: '6.66%'}} >
					{ board['6-1'] && board['6-1'].length ? <Goti move={move} setChoice={setChoice} place="6-1" glow={board['6-1'].includes(turn) && !choice} color={colors[board['6-1']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', left: '13.32%'}}>
					{ board['6-2'] && board['6-2'].length ? <Goti move={move} setChoice={setChoice} place="6-2" glow={board['6-2'].includes(turn) && !choice} color={colors[board['6-2']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', left: '19.98%'}}>
					{ board['6-3'] && board['6-3'].length ? <Goti move={move} setChoice={setChoice} place="6-3" glow={board['6-3'].includes(turn) && !choice} color={colors[board['6-3']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', left: '26.64%'}}>
					{ board['6-4'] && board['6-4'].length ? <Goti move={move} setChoice={setChoice} place="6-4" glow={board['6-4'].includes(turn) && !choice} color={colors[board['6-4']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', left: '33.3%'}}>
					{ board['6-5'] && board['6-5'].length ? <Goti move={move} setChoice={setChoice} place="6-5" glow={board['6-5'].includes(turn) && !choice} color={colors[board['6-5']]} /> : null }
				</div>
				<div className="cells" style={{top: 0, left: '40%'}}>
					{ board['0-6'] && board['0-6'].length ? <Goti move={move} setChoice={setChoice} place="0-6" glow={board['0-6'].includes(turn) && !choice} color={colors[board['0-6']]} /> : null }
				</div>
				<div className="cells" style={{top: '6.66%', left: '40%'}}>
					{ board['1-6'] && board['1-6'].length ? <Goti move={move} setChoice={setChoice} place="1-6" glow={board['1-6'].includes(turn) && !choice} color={colors[board['1-6']]} /> : null }
				</div>
				<div className="cells safe" style={{top: '13.32%', left: '40%'}} />
				<div className="cells" style={{top: '13.32%', left: '40%'}}>
					{ board['2-6'] && board['2-6'].length ? <Goti move={move} setChoice={setChoice} place="2-6" glow={board['2-6'].includes(turn) && !choice} color={colors[board['2-6']]} /> : null }
				</div>
				<div className="cells" style={{top: '19.98%', left: '40%'}}>
					{ board['3-6'] && board['3-6'].length ? <Goti move={move} setChoice={setChoice} place="3-6" glow={board['3-6'].includes(turn) && !choice} color={colors[board['3-6']]} /> : null }
				</div>
				<div className="cells" style={{top: '26.64%', left: '40%'}}>
					{ board['4-6'] && board['4-6'].length ? <Goti move={move} setChoice={setChoice} place="4-6" glow={board['4-6'].includes(turn) && !choice} color={colors[board['4-6']]} /> : null }
				</div>
				<div className="cells" style={{top: '33.3%', left: '40%'}}>
					{ board['5-6'] && board['5-6'].length ? <Goti move={move} setChoice={setChoice} place="5-6" glow={board['5-6'].includes(turn) && !choice} color={colors[board['5-6']]} /> : null }
				</div>
				<div className="cells" style={{top: 0, left: '46.66%'}}>
					{ board['0-7'] && board['0-7'].length ? <Goti move={move} setChoice={setChoice} place="0-7" glow={board['0-7'].includes(turn) && !choice} color={colors[board['0-7']]} /> : null }
				</div>
				<div className="cells yellow" style={{top: '6.66%', left: '46.66%'}}>
					{ board['1-7'] && board['1-7'].length ? <Goti move={move} setChoice={setChoice} place="1-7" glow={board['1-7'].includes(turn) && !choice} color={colors[board['1-7']]} /> : null }
				</div>
				<div className="cells yellow" style={{top: '13.32%', left: '46.66%'}}>
					{ board['2-7'] && board['2-7'].length ? <Goti move={move} setChoice={setChoice} place="2-7" glow={board['2-7'].includes(turn) && !choice} color={colors[board['2-7']]} /> : null }
				</div>
				<div className="cells yellow" style={{top: '19.98%', left: '46.66%'}}>
					{ board['3-7'] && board['3-7'].length ? <Goti move={move} setChoice={setChoice} place="3-7" glow={board['3-7'].includes(turn) && !choice} color={colors[board['3-7']]} /> : null }
				</div>
				<div className="cells yellow" style={{top: '26.64%', left: '46.66%'}}>
					{ board['4-7'] && board['4-7'].length ? <Goti move={move} setChoice={setChoice} place="4-7" glow={board['4-7'].includes(turn) && !choice} color={colors[board['4-7']]} /> : null }
				</div>
				<div className="cells yellow" style={{top: '33.3%', left: '46.66%'}}>
					{ board['5-7'] && board['5-7'].length ? <Goti move={move} setChoice={setChoice} place="5-7" glow={board['5-7'].includes(turn) && !choice} color={colors[board['5-7']]} /> : null }
				</div>
				<div className="cells" style={{top: 0, left: '53.32%'}}>
					{ board['0-8'] && board['0-8'].length ? <Goti move={move} setChoice={setChoice} place="0-8" glow={board['0-8'].includes(turn) && !choice} color={colors[board['0-8']]} /> : null }
				</div>
				<div className="cells y-start" style={{top: '6.66%', left: '53.32%'}} />
				<div className="cells" style={{top: '6.66%', left: '53.32%'}}>
					{ board['1-8'] && board['1-8'].length ? <Goti move={move} setChoice={setChoice} place="1-8" glow={board['1-8'].includes(turn) && !choice} color={colors[board['1-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '13.32%', left: '53.32%'}}>
					{ board['2-8'] && board['2-8'].length ? <Goti move={move} setChoice={setChoice} place="2-8" glow={board['2-8'].includes(turn) && !choice} color={colors[board['2-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '19.98%', left: '53.32%'}}>
					{ board['3-8'] && board['3-8'].length ? <Goti move={move} setChoice={setChoice} place="3-8" glow={board['3-8'].includes(turn) && !choice} color={colors[board['3-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '26.64%', left: '53.32%'}}>
					{ board['4-8'] && board['4-8'].length ? <Goti move={move} setChoice={setChoice} place="4-8" glow={board['4-8'].includes(turn) && !choice} color={colors[board['4-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '33.3%', left: '53.32%'}}>
					{ board['5-8'] && board['5-8'].length ? <Goti move={move} setChoice={setChoice} place="5-8" glow={board['5-8'].includes(turn) && !choice} color={colors[board['5-8']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', right: '33.3%'}}>
					{ board['6-9'] && board['6-9'].length ? <Goti move={move} setChoice={setChoice} place="6-9" glow={board['6-9'].includes(turn) && !choice} color={colors[board['6-9']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', right: '26.64%'}}>
					{ board['6-10'] && board['6-10'].length ? <Goti move={move} setChoice={setChoice} place="6-10" glow={board['6-10'].includes(turn) && !choice} color={colors[board['6-10']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', right: '19.98%'}}>
					{ board['6-11'] && board['6-11'].length ? <Goti move={move} setChoice={setChoice} place="6-11" glow={board['6-11'].includes(turn) && !choice} color={colors[board['6-11']]} /> : null }
				</div>
				<div className="cells safe" style={{top: '40%', right: '13.32%'}} />
				<div className="cells" style={{top: '40%', right: '13.32%'}}>
					{ board['6-12'] && board['6-12'].length ? <Goti move={move} setChoice={setChoice} place="6-12" glow={board['6-12'].includes(turn) && !choice} color={colors[board['6-12']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', right: '6.66%'}}>
					{ board['6-13'] && board['6-13'].length ? <Goti move={move} setChoice={setChoice} place="6-13" glow={board['6-13'].includes(turn) && !choice} color={colors[board['6-13']]} /> : null }
				</div>
				<div className="cells" style={{top: '40%', right: 0}}>
					{ board['6-14'] && board['6-14'].length ? <Goti move={move} setChoice={setChoice} place="6-14" glow={board['6-14'].includes(turn) && !choice} color={colors[board['6-14']]} /> : null }
				</div>
				<div className="cells blue" style={{top: '46.66%', right: '33.3%'}}>
					{ board['7-9'] && board['7-9'].length ? <Goti move={move} setChoice={setChoice} place="7-9" glow={board['7-9'].includes(turn) && !choice} color={colors[board['7-9']]} /> : null }
				</div>
				<div className="cells blue" style={{top: '46.66%', right: '26.64%'}}>
					{ board['7-10'] && board['7-10'].length ? <Goti move={move} setChoice={setChoice} place="7-10" glow={board['7-10'].includes(turn) && !choice} color={colors[board['7-10']]} /> : null }
				</div>
				<div className="cells blue" style={{top: '46.66%', right: '19.98%'}}>
					{ board['7-11'] && board['7-11'].length ? <Goti move={move} setChoice={setChoice} place="7-11" glow={board['7-11'].includes(turn) && !choice} color={colors[board['7-11']]} /> : null }
				</div>
				<div className="cells blue" style={{top: '46.66%', right: '13.32%'}}>
					{ board['7-12'] && board['7-12'].length ? <Goti move={move} setChoice={setChoice} place="7-12" glow={board['7-12'].includes(turn) && !choice} color={colors[board['7-12']]} /> : null }
				</div>
				<div className="cells blue" style={{top: '46.66%', right: '6.66%'}}>
					{ board['7-13'] && board['7-13'].length ? <Goti move={move} setChoice={setChoice} place="7-13" glow={board['7-13'].includes(turn) && !choice} color={colors[board['7-13']]} /> : null }
				</div>
				<div className="cells" style={{top: '46.66%', right: 0}}>
					{ board['7-14'] && board['7-14'].length ? <Goti move={move} setChoice={setChoice} place="7-14" glow={board['7-14'].includes(turn) && !choice} color={colors[board['7-14']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', right: '33.3%'}}>
					{ board['8-9'] && board['8-9'].length ? <Goti move={move} setChoice={setChoice} place="8-9" glow={board['8-9'].includes(turn) && !choice} color={colors[board['8-9']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', right: '26.64%'}}>
					{ board['8-10'] && board['8-10'].length ? <Goti move={move} setChoice={setChoice} place="8-10" glow={board['8-10'].includes(turn) && !choice} color={colors[board['8-10']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', right: '19.98%'}}>
					{ board['8-11'] && board['8-11'].length ? <Goti move={move} setChoice={setChoice} place="8-11" glow={board['8-11'].includes(turn) && !choice} color={colors[board['8-11']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', right: '13.32%'}}>
					{ board['8-12'] && board['8-12'].length ? <Goti move={move} setChoice={setChoice} place="8-12" glow={board['8-12'].includes(turn) && !choice} color={colors[board['8-12']]} /> : null }
				</div>
				<div className="cells b-start" style={{top: '53.32%', right: '6.66%'}} />
				<div className="cells" style={{top: '53.32%', right: '6.66%'}}>
					{ board['8-13'] && board['8-13'].length ? <Goti move={move} setChoice={setChoice} place="8-13" glow={board['8-13'].includes(turn) && !choice} color={colors[board['8-13']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', right: 0}}>
					{ board['8-14'] && board['8-14'].length ? <Goti move={move} setChoice={setChoice} place="8-14" glow={board['8-14'].includes(turn) && !choice} color={colors[board['8-14']]} /> : null }
				</div>
				<div className="cells" style={{bottom: 0, left: '53.32%'}}>
					{ board['14-8'] && board['14-8'].length ? <Goti move={move} setChoice={setChoice} place="14-8" glow={board['14-8'].includes(turn) && !choice} color={colors[board['14-8']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '6.66%', left: '53.32%'}}>
					{ board['13-8'] && board['13-8'].length ? <Goti move={move} setChoice={setChoice} place="13-8" glow={board['13-8'].includes(turn) && !choice} color={colors[board['13-8']]} /> : null }
				</div>
				<div className="cells safe" style={{bottom: '13.32%', left: '53.32%'}} />
				<div className="cells" style={{bottom: '13.32%', left: '53.32%'}}>
					{ board['12-8'] && board['12-8'].length ? <Goti move={move} setChoice={setChoice} place="12-8" glow={board['12-8'].includes(turn) && !choice} color={colors[board['12-8']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '19.98%', left: '53.32%'}}>
					{ board['11-8'] && board['11-8'].length ? <Goti move={move} setChoice={setChoice} place="11-8" glow={board['11-8'].includes(turn) && !choice} color={colors[board['11-8']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '26.64%', left: '53.32%'}}>
					{ board['10-8'] && board['10-8'].length ? <Goti move={move} setChoice={setChoice} place="10-8" glow={board['10-8'].includes(turn) && !choice} color={colors[board['10-8']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '33.3%', left: '53.32%'}}>
					{ board['9-8'] && board['9-8'].length ? <Goti move={move} setChoice={setChoice} place="9-8" glow={board['9-8'].includes(turn) && !choice} color={colors[board['9-8']]} /> : null }
				</div>
				<div className="cells" style={{bottom: 0, left: '46.66%'}}>
					{ board['14-7'] && board['14-7'].length ? <Goti move={move} setChoice={setChoice} place="14-7" glow={board['14-7'].includes(turn) && !choice} color={colors[board['14-7']]} /> : null }
				</div>
				<div className="cells red " style={{bottom: '6.66%', left: '46.66%'}}>
					{ board['13-7'] && board['13-7'].length ? <Goti move={move} setChoice={setChoice} place="13-7" glow={board['13-7'].includes(turn) && !choice} color={colors[board['13-7']]} /> : null }
				</div>
				<div className="cells red" style={{bottom: '13.32%', left: '46.66%'}}>
					{ board['12-7'] && board['12-7'].length ? <Goti move={move} setChoice={setChoice} place="12-7" glow={board['12-7'].includes(turn) && !choice} color={colors[board['12-7']]} /> : null }
				</div>
				<div className="cells red" style={{bottom: '19.98%', left: '46.66%'}}>
					{ board['11-7'] && board['11-7'].length ? <Goti move={move} setChoice={setChoice} place="11-7" glow={board['11-7'].includes(turn) && !choice} color={colors[board['11-7']]} /> : null }
				</div>
				<div className="cells red" style={{bottom: '26.64%', left: '46.66%'}}>
					{ board['10-7'] && board['10-7'].length ? <Goti move={move} setChoice={setChoice} place="10-7" glow={board['10-7'].includes(turn) && !choice} color={colors[board['10-7']]} /> : null }
				</div>
				<div className="cells red" style={{bottom: '33.3%', left: '46.66%'}} />
					{ board['9-7'] && board['9-7'].length ? <Goti move={move} setChoice={setChoice} place="9-7" glow={board['9-7'].includes(turn) && !choice} color={colors[board['9-7']]} /> : null }
				<div className="cells" style={{bottom: 0, left: '40%'}}>
					{ board['14-6'] && board['14-6'].length ? <Goti move={move} setChoice={setChoice} place="14-6" glow={board['14-6'].includes(turn) && !choice} color={colors[board['14-6']]} /> : null }
				</div>
				<div className="cells r-start" style={{bottom: '6.66%', left: '40%'}} />
				<div className="cells" style={{bottom: '6.66%', left: '40%'}}>
					{ board['13-6'] && board['13-6'].length ? <Goti move={move} setChoice={setChoice} place="13-6" glow={board['13-6'].includes(turn) && !choice} color={colors[board['13-6']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '13.32%', left: '40%'}}>
					{ board['12-6'] && board['12-6'].length ? <Goti move={move} setChoice={setChoice} place="12-6" glow={board['12-6'].includes(turn) && !choice} color={colors[board['12-6']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '19.98%', left: '40%'}}>
					{ board['11-6'] && board['11-6'].length ? <Goti move={move} setChoice={setChoice} place="11-6" glow={board['11-6'].includes(turn) && !choice} color={colors[board['11-6']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '26.64%', left: '40%'}}>
					{ board['10-6'] && board['10-6'].length ? <Goti move={move} setChoice={setChoice} place="10-6" glow={board['10-6'].includes(turn) && !choice} color={colors[board['10-6']]} /> : null }
				</div>
				<div className="cells" style={{bottom: '33.3%', left: '40%'}}>
					{ board['9-6'] && board['9-6'].length ? <Goti move={move} setChoice={setChoice} place="9-6" glow={board['9-6'].includes(turn) && !choice} color={colors[board['9-6']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', left: '33.3%'}}>
					{ board['8-5'] && board['8-5'].length ? <Goti move={move} setChoice={setChoice} place="8-5" glow={board['8-5'].includes(turn) && !choice} color={colors[board['8-5']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', left: '26.64%'}}>
					{ board['8-4'] && board['8-4'].length ? <Goti move={move} setChoice={setChoice} place="8-4" glow={board['8-4'].includes(turn) && !choice} color={colors[board['8-4']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', left: '19.98%'}}>
					{ board['8-3'] && board['8-3'].length ? <Goti move={move} setChoice={setChoice} place="8-3" glow={board['8-3'].includes(turn) && !choice} color={colors[board['8-3']]} /> : null }
				</div>
				<div className="cells safe" style={{top: '53.32%', left: '13.32%'}} />
				<div className="cells" style={{top: '53.32%', left: '13.32%'}}>
					{ board['8-2'] && board['8-2'].length ? <Goti move={move} setChoice={setChoice} place="8-2" glow={board['8-2'].includes(turn) && !choice} color={colors[board['8-2']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', left: '6.66%'}}>
					{ board['8-1'] && board['8-1'].length ? <Goti move={move} setChoice={setChoice} place="8-1" glow={board['8-1'].includes(turn) && !choice} color={colors[board['8-1']]} /> : null }
				</div>
				<div className="cells" style={{top: '53.32%', left: 0}}>
					{ board['8-0'] && board['8-0'].length ? <Goti move={move} setChoice={setChoice} place="8-0" glow={board['8-0'].includes(turn) && !choice} color={colors[board['8-0']]} /> : null }
				</div>
				<div className="cells green" style={{top: '46.66%', left: '33.3%'}}>
					{ board['7-5'] && board['7-5'].length ? <Goti move={move} setChoice={setChoice} place="7-5" glow={board['7-5'].includes(turn) && !choice} color={colors[board['7-5']]} /> : null }
				</div>
				<div className="cells green" style={{top: '46.66%', left: '26.64%'}}>
					{ board['7-4'] && board['7-4'].length ? <Goti move={move} setChoice={setChoice} place="7-4" glow={board['7-4'].includes(turn) && !choice} color={colors[board['7-4']]} /> : null }
				</div>
				<div className="cells green" style={{top: '46.66%', left: '19.98%'}}>
					{ board['7-3'] && board['7-3'].length ? <Goti move={move} setChoice={setChoice} place="7-3" glow={board['7-3'].includes(turn) && !choice} color={colors[board['7-3']]} /> : null }
				</div>
				<div className="cells green" style={{top: '46.66%', left: '13.32%'}}>
					{ board['7-2'] && board['7-2'].length ? <Goti move={move} setChoice={setChoice} place="7-2" glow={board['7-2'].includes(turn) && !choice} color={colors[board['7-2']]} /> : null }
				</div>
				<div className="cells green" style={{top: '46.66%', left: '6.66%'}}>
					{ board['7-1'] && board['7-1'].length ? <Goti move={move} setChoice={setChoice} place="7-1" glow={board['7-1'].includes(turn) && !choice} color={colors[board['7-1']]} /> : null }
				</div>
				<div className="cells" style={{top: '46.66%', left: 0}}>
					{ board['7-0'] && board['7-0'].length ? <Goti move={move} setChoice={setChoice} place="7-0" glow={board['7-0'].includes(turn) && !choice} color={colors[board['7-0']]} /> : null }
				</div>
			</div>
			<div className="panel">
				<div className="info">
					<table style={{ margin: "0 auto"}}>
						<thead>
							<tr>
								<th>Player</th>
								<th>Gotis Left</th>
							</tr>
						</thead>
						<tbody>
							<tr>
								<td>Green</td>
								<td>{positions[0].reduce((total, curr) => curr === true ? total-1 : total, 4)}</td>
							</tr>
							<tr>
								<td>Yellow</td>
								<td>{positions[1].reduce((total, curr) => curr === true ? total-1 : total, 4)}</td>
							</tr>
							<tr>
								<td>Blue</td>
								<td>{positions[2].reduce((total, curr) => curr === true ? total-1 : total, 4)}</td>
							</tr>
							<tr>
								<td>Red</td>
								<td>{positions[3].reduce((total, curr) => curr === true ? total-1 : total, 4)}</td>
							</tr>
						</tbody>
					</table>
				</div>
				<div className="dice">
					<Turn turn={turns[turn]} color={colors[turn]} choice={choice} />
					<ol className="die-list even-roll" data-roll="1" style={{ cursor: choice && die ? "pointer" : null }} onClick={() => { rollDice(turn) }}>
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

function toggleClasses(die) {
	die.classList.toggle("odd-roll");
	die.classList.toggle("even-roll");
}

export default App;

// Board Credits: https://codepen.io/thewayur/pen/WNNXJMO
