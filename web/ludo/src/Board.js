import React from 'react'
import Goti from './Goti'

export default function Board(props) {
    return (
        <div className="game">
			<div className="house green">
				<div className="box">
					<div className="square square-one green">
						{props.positions[0].reduce((total, curr) => total + (curr === false), 0) > 0 ? <Goti length={1} color={props.colors[0]} /> : null}
					</div>
					<div className="square square-two green">
						{props.positions[0].reduce((total, curr) => total + (curr === false), 0) > 1 ? <Goti length={1} color={props.colors[0]} /> : null}
					</div>
					<div className="square square-three green">
						{props.positions[0].reduce((total, curr) => total + (curr === false), 0) > 2 ? <Goti length={1} color={props.colors[0]} /> : null}
					</div>
					<div className="square square-four green">
						{props.positions[0].reduce((total, curr) => total + (curr === false), 0) > 3 ? <Goti length={1} color={props.colors[0]} /> : null}
					</div>
				</div>
			</div>
			<div className="house yellow" style={{right: 0}}>
				<div className="box">
					<div className="square square-one yellow">
						{props.positions[1].reduce((total, curr) => total + (curr === false), 0) > 0 ? <Goti length={1} color={props.colors[1]} /> : null}
					</div>
					<div className="square square-two yellow">
						{props.positions[1].reduce((total, curr) => total + (curr === false), 0) > 1 ? <Goti length={1} color={props.colors[1]} /> : null}
					</div>
					<div className="square square-three yellow">
						{props.positions[1].reduce((total, curr) => total + (curr === false), 0) > 2 ? <Goti length={1} color={props.colors[1]} /> : null}
					</div>
					<div className="square square-four yellow">
						{props.positions[1].reduce((total, curr) => total + (curr === false), 0) > 3 ? <Goti length={1} color={props.colors[1]} /> : null}
					</div>
				</div>
			</div>
			<div className="house red" style={{bottom: 0}}>
				<div className="box">
					<div className="square square-one red">
						{props.positions[3].reduce((total, curr) => total + (curr === false), 0) > 0 ? <Goti length={1} color={props.colors[3]} /> : null}
					</div>
					<div className="square square-two red">
						{props.positions[3].reduce((total, curr) => total + (curr === false), 0) > 1 ? <Goti length={1} color={props.colors[3]} /> : null}
					</div>
					<div className="square square-three red">
						{props.positions[3].reduce((total, curr) => total + (curr === false), 0) > 2 ? <Goti length={1} color={props.colors[3]} /> : null}
					</div>
					<div className="square square-four red">
						{props.positions[3].reduce((total, curr) => total + (curr === false), 0) > 3 ? <Goti length={1} color={props.colors[3]} /> : null}
					</div>
				</div>
			</div>
			<div className="house blue" style={{bottom: 0, right: 0}}>
				<div className="box">
					<div className="square square-one blue">
						{props.positions[2].reduce((total, curr) => total + (curr === false), 0) > 0 ? <Goti length={1} color={props.colors[2]} /> : null}
					</div>
					<div className="square square-two blue">
						{props.positions[2].reduce((total, curr) => total + (curr === false), 0) > 1 ? <Goti length={1} color={props.colors[2]} /> : null}
					</div>
					<div className="square square-three blue">
						{props.positions[2].reduce((total, curr) => total + (curr === false), 0) > 2 ? <Goti length={1} color={props.colors[2]} /> : null}
					</div>
					<div className="square square-four blue">
						{props.positions[2].reduce((total, curr) => total + (curr === false), 0) > 3 ? <Goti length={1} color={props.colors[2]} /> : null}
					</div>
				</div>
			</div>
			<div className="home" />
			<div className="cells" style={{top: '40%'}}>
				{ props.board['6-0'] && props.board['6-0'].length ? <Goti length={props.board['6-0'].length} move={props.move} setChoice={props.setChoice} place="6-0" glow={props.board['6-0'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-0'][0]]} /> : null }
			</div>
			<div className="cells g-start" style={{top: '40%', left: '6.66%'}} />
			<div className="cells" style={{top: '40%', left: '6.66%'}} >
				{ props.board['6-1'] && props.board['6-1'].length ? <Goti length={props.board['6-1'].length} move={props.move} setChoice={props.setChoice} place="6-1" glow={props.board['6-1'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-1'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', left: '13.32%'}}>
				{ props.board['6-2'] && props.board['6-2'].length ? <Goti length={props.board['6-2'].length} move={props.move} setChoice={props.setChoice} place="6-2" glow={props.board['6-2'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-2'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', left: '19.98%'}}>
				{ props.board['6-3'] && props.board['6-3'].length ? <Goti length={props.board['6-3'].length} move={props.move} setChoice={props.setChoice} place="6-3" glow={props.board['6-3'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-3'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', left: '26.64%'}}>
				{ props.board['6-4'] && props.board['6-4'].length ? <Goti length={props.board['6-4'].length} move={props.move} setChoice={props.setChoice} place="6-4" glow={props.board['6-4'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-4'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', left: '33.3%'}}>
				{ props.board['6-5'] && props.board['6-5'].length ? <Goti length={props.board['6-5'].length} move={props.move} setChoice={props.setChoice} place="6-5" glow={props.board['6-5'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-5'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: 0, left: '40%'}}>
				{ props.board['0-6'] && props.board['0-6'].length ? <Goti length={props.board['0-6'].length} move={props.move} setChoice={props.setChoice} place="0-6" glow={props.board['0-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['0-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '6.66%', left: '40%'}}>
				{ props.board['1-6'] && props.board['1-6'].length ? <Goti length={props.board['1-6'].length} move={props.move} setChoice={props.setChoice} place="1-6" glow={props.board['1-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['1-6'][0]]} /> : null }
			</div>
			<div className="cells safe" style={{top: '13.32%', left: '40%'}} />
			<div className="cells" style={{top: '13.32%', left: '40%'}}>
				{ props.board['2-6'] && props.board['2-6'].length ? <Goti length={props.board['2-6'].length} move={props.move} setChoice={props.setChoice} place="2-6" glow={props.board['2-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['2-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '19.98%', left: '40%'}}>
				{ props.board['3-6'] && props.board['3-6'].length ? <Goti length={props.board['3-6'].length} move={props.move} setChoice={props.setChoice} place="3-6" glow={props.board['3-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['3-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '26.64%', left: '40%'}}>
				{ props.board['4-6'] && props.board['4-6'].length ? <Goti length={props.board['4-6'].length} move={props.move} setChoice={props.setChoice} place="4-6" glow={props.board['4-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['4-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '33.3%', left: '40%'}}>
				{ props.board['5-6'] && props.board['5-6'].length ? <Goti length={props.board['5-6'].length} move={props.move} setChoice={props.setChoice} place="5-6" glow={props.board['5-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['5-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: 0, left: '46.66%'}}>
				{ props.board['0-7'] && props.board['0-7'].length ? <Goti length={props.board['0-7'].length} move={props.move} setChoice={props.setChoice} place="0-7" glow={props.board['0-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['0-7'][0]]} /> : null }
			</div>
			<div className="cells yellow" style={{top: '6.66%', left: '46.66%'}}>
				{ props.board['1-7'] && props.board['1-7'].length ? <Goti length={props.board['1-7'].length} move={props.move} setChoice={props.setChoice} place="1-7" glow={props.board['1-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['1-7'][0]]} /> : null }
			</div>
			<div className="cells yellow" style={{top: '13.32%', left: '46.66%'}}>
				{ props.board['2-7'] && props.board['2-7'].length ? <Goti length={props.board['2-7'].length} move={props.move} setChoice={props.setChoice} place="2-7" glow={props.board['2-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['2-7'][0]]} /> : null }
			</div>
			<div className="cells yellow" style={{top: '19.98%', left: '46.66%'}}>
				{ props.board['3-7'] && props.board['3-7'].length ? <Goti length={props.board['3-7'].length} move={props.move} setChoice={props.setChoice} place="3-7" glow={props.board['3-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['3-7'][0]]} /> : null }
			</div>
			<div className="cells yellow" style={{top: '26.64%', left: '46.66%'}}>
				{ props.board['4-7'] && props.board['4-7'].length ? <Goti length={props.board['4-7'].length} move={props.move} setChoice={props.setChoice} place="4-7" glow={props.board['4-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['4-7'][0]]} /> : null }
			</div>
			<div className="cells yellow" style={{top: '33.3%', left: '46.66%'}}>
				{ props.board['5-7'] && props.board['5-7'].length ? <Goti length={props.board['5-7'].length} move={props.move} setChoice={props.setChoice} place="5-7" glow={props.board['5-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['5-7'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: 0, left: '53.32%'}}>
				{ props.board['0-8'] && props.board['0-8'].length ? <Goti length={props.board['0-8'].length} move={props.move} setChoice={props.setChoice} place="0-8" glow={props.board['0-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['0-8'][0]]} /> : null }
			</div>
			<div className="cells y-start" style={{top: '6.66%', left: '53.32%'}} />
			<div className="cells" style={{top: '6.66%', left: '53.32%'}}>
				{ props.board['1-8'] && props.board['1-8'].length ? <Goti length={props.board['1-8'].length} move={props.move} setChoice={props.setChoice} place="1-8" glow={props.board['1-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['1-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '13.32%', left: '53.32%'}}>
				{ props.board['2-8'] && props.board['2-8'].length ? <Goti length={props.board['2-8'].length} move={props.move} setChoice={props.setChoice} place="2-8" glow={props.board['2-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['2-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '19.98%', left: '53.32%'}}>
				{ props.board['3-8'] && props.board['3-8'].length ? <Goti length={props.board['3-8'].length} move={props.move} setChoice={props.setChoice} place="3-8" glow={props.board['3-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['3-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '26.64%', left: '53.32%'}}>
				{ props.board['4-8'] && props.board['4-8'].length ? <Goti length={props.board['4-8'].length} move={props.move} setChoice={props.setChoice} place="4-8" glow={props.board['4-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['4-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '33.3%', left: '53.32%'}}>
				{ props.board['5-8'] && props.board['5-8'].length ? <Goti length={props.board['5-8'].length} move={props.move} setChoice={props.setChoice} place="5-8" glow={props.board['5-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['5-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', right: '33.3%'}}>
				{ props.board['6-9'] && props.board['6-9'].length ? <Goti length={props.board['6-9'].length} move={props.move} setChoice={props.setChoice} place="6-9" glow={props.board['6-9'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-9'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', right: '26.64%'}}>
				{ props.board['6-10'] && props.board['6-10'].length ? <Goti length={props.board['6-10'].length} move={props.move} setChoice={props.setChoice} place="6-10" glow={props.board['6-10'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-10'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', right: '19.98%'}}>
				{ props.board['6-11'] && props.board['6-11'].length ? <Goti length={props.board['6-11'].length} move={props.move} setChoice={props.setChoice} place="6-11" glow={props.board['6-11'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-11'][0]]} /> : null }
			</div>
			<div className="cells safe" style={{top: '40%', right: '13.32%'}} />
			<div className="cells" style={{top: '40%', right: '13.32%'}}>
				{ props.board['6-12'] && props.board['6-12'].length ? <Goti length={props.board['6-12'].length} move={props.move} setChoice={props.setChoice} place="6-12" glow={props.board['6-12'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-12'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', right: '6.66%'}}>
				{ props.board['6-13'] && props.board['6-13'].length ? <Goti length={props.board['6-13'].length} move={props.move} setChoice={props.setChoice} place="6-13" glow={props.board['6-13'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-13'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '40%', right: 0}}>
				{ props.board['6-14'] && props.board['6-14'].length ? <Goti length={props.board['6-14'].length} move={props.move} setChoice={props.setChoice} place="6-14" glow={props.board['6-14'].includes(props.turn) && !props.choice} color={props.colors[props.board['6-14'][0]]} /> : null }
			</div>
			<div className="cells blue" style={{top: '46.66%', right: '33.3%'}}>
				{ props.board['7-9'] && props.board['7-9'].length ? <Goti length={props.board['7-9'].length} move={props.move} setChoice={props.setChoice} place="7-9" glow={props.board['7-9'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-9'][0]]} /> : null }
			</div>
			<div className="cells blue" style={{top: '46.66%', right: '26.64%'}}>
				{ props.board['7-10'] && props.board['7-10'].length ? <Goti length={props.board['7-10'].length} move={props.move} setChoice={props.setChoice} place="7-10" glow={props.board['7-10'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-10'][0]]} /> : null }
			</div>
			<div className="cells blue" style={{top: '46.66%', right: '19.98%'}}>
				{ props.board['7-11'] && props.board['7-11'].length ? <Goti length={props.board['7-11'].length} move={props.move} setChoice={props.setChoice} place="7-11" glow={props.board['7-11'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-11'][0]]} /> : null }
			</div>
			<div className="cells blue" style={{top: '46.66%', right: '13.32%'}}>
				{ props.board['7-12'] && props.board['7-12'].length ? <Goti length={props.board['7-12'].length} move={props.move} setChoice={props.setChoice} place="7-12" glow={props.board['7-12'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-12'][0]]} /> : null }
			</div>
			<div className="cells blue" style={{top: '46.66%', right: '6.66%'}}>
				{ props.board['7-13'] && props.board['7-13'].length ? <Goti length={props.board['7-13'].length} move={props.move} setChoice={props.setChoice} place="7-13" glow={props.board['7-13'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-13'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '46.66%', right: 0}}>
				{ props.board['7-14'] && props.board['7-14'].length ? <Goti length={props.board['7-14'].length} move={props.move} setChoice={props.setChoice} place="7-14" glow={props.board['7-14'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-14'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', right: '33.3%'}}>
				{ props.board['8-9'] && props.board['8-9'].length ? <Goti length={props.board['8-9'].length} move={props.move} setChoice={props.setChoice} place="8-9" glow={props.board['8-9'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-9'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', right: '26.64%'}}>
				{ props.board['8-10'] && props.board['8-10'].length ? <Goti length={props.board['8-10'].length} move={props.move} setChoice={props.setChoice} place="8-10" glow={props.board['8-10'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-10'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', right: '19.98%'}}>
				{ props.board['8-11'] && props.board['8-11'].length ? <Goti length={props.board['8-11'].length} move={props.move} setChoice={props.setChoice} place="8-11" glow={props.board['8-11'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-11'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', right: '13.32%'}}>
				{ props.board['8-12'] && props.board['8-12'].length ? <Goti length={props.board['8-12'].length} move={props.move} setChoice={props.setChoice} place="8-12" glow={props.board['8-12'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-12'][0]]} /> : null }
			</div>
			<div className="cells b-start" style={{top: '53.32%', right: '6.66%'}} />
			<div className="cells" style={{top: '53.32%', right: '6.66%'}}>
				{ props.board['8-13'] && props.board['8-13'].length ? <Goti length={props.board['8-13'].length} move={props.move} setChoice={props.setChoice} place="8-13" glow={props.board['8-13'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-13'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', right: 0}}>
				{ props.board['8-14'] && props.board['8-14'].length ? <Goti length={props.board['8-14'].length} move={props.move} setChoice={props.setChoice} place="8-14" glow={props.board['8-14'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-14'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: 0, left: '53.32%'}}>
				{ props.board['14-8'] && props.board['14-8'].length ? <Goti length={props.board['14-8'].length} move={props.move} setChoice={props.setChoice} place="14-8" glow={props.board['14-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['14-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: '6.66%', left: '53.32%'}}>
				{ props.board['13-8'] && props.board['13-8'].length ? <Goti length={props.board['13-8'].length} move={props.move} setChoice={props.setChoice} place="13-8" glow={props.board['13-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['13-8'][0]]} /> : null }
			</div>
			<div className="cells safe" style={{bottom: '13.32%', left: '53.32%'}} />
			<div className="cells" style={{bottom: '13.32%', left: '53.32%'}}>
				{ props.board['12-8'] && props.board['12-8'].length ? <Goti length={props.board['12-8'].length} move={props.move} setChoice={props.setChoice} place="12-8" glow={props.board['12-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['12-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: '19.98%', left: '53.32%'}}>
				{ props.board['11-8'] && props.board['11-8'].length ? <Goti length={props.board['11-8'].length} move={props.move} setChoice={props.setChoice} place="11-8" glow={props.board['11-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['11-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: '26.64%', left: '53.32%'}}>
				{ props.board['10-8'] && props.board['10-8'].length ? <Goti length={props.board['10-8'].length} move={props.move} setChoice={props.setChoice} place="10-8" glow={props.board['10-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['10-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: '33.3%', left: '53.32%'}}>
				{ props.board['9-8'] && props.board['9-8'].length ? <Goti length={props.board['9-8'].length} move={props.move} setChoice={props.setChoice} place="9-8" glow={props.board['9-8'].includes(props.turn) && !props.choice} color={props.colors[props.board['9-8'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: 0, left: '46.66%'}}>
				{ props.board['14-7'] && props.board['14-7'].length ? <Goti length={props.board['14-7'].length} move={props.move} setChoice={props.setChoice} place="14-7" glow={props.board['14-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['14-7'][0]]} /> : null }
			</div>
			<div className="cells red " style={{bottom: '6.66%', left: '46.66%'}}>
				{ props.board['13-7'] && props.board['13-7'].length ? <Goti length={props.board['13-7'].length} move={props.move} setChoice={props.setChoice} place="13-7" glow={props.board['13-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['13-7'][0]]} /> : null }
			</div>
			<div className="cells red" style={{bottom: '13.32%', left: '46.66%'}}>
				{ props.board['12-7'] && props.board['12-7'].length ? <Goti length={props.board['12-7'].length} move={props.move} setChoice={props.setChoice} place="12-7" glow={props.board['12-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['12-7'][0]]} /> : null }
			</div>
			<div className="cells red" style={{bottom: '19.98%', left: '46.66%'}}>
				{ props.board['11-7'] && props.board['11-7'].length ? <Goti length={props.board['11-7'].length} move={props.move} setChoice={props.setChoice} place="11-7" glow={props.board['11-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['11-7'][0]]} /> : null }
			</div>
			<div className="cells red" style={{bottom: '26.64%', left: '46.66%'}}>
				{ props.board['10-7'] && props.board['10-7'].length ? <Goti length={props.board['10-7'].length} move={props.move} setChoice={props.setChoice} place="10-7" glow={props.board['10-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['10-7'][0]]} /> : null }
			</div>
			<div className="cells red" style={{bottom: '33.3%', left: '46.66%'}}>
				{ props.board['9-7'] && props.board['9-7'].length ? <Goti length={props.board['9-7'].length} move={props.move} setChoice={props.setChoice} place="9-7" glow={props.board['9-7'].includes(props.turn) && !props.choice} color={props.colors[props.board['9-7'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: 0, left: '40%'}}>
				{ props.board['14-6'] && props.board['14-6'].length ? <Goti length={props.board['14-6'].length} move={props.move} setChoice={props.setChoice} place="14-6" glow={props.board['14-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['14-6'][0]]} /> : null }
			</div>
			<div className="cells r-start" style={{bottom: '6.66%', left: '40%'}} />
			<div className="cells" style={{bottom: '6.66%', left: '40%'}}>
				{ props.board['13-6'] && props.board['13-6'].length ? <Goti length={props.board['13-6'].length} move={props.move} setChoice={props.setChoice} place="13-6" glow={props.board['13-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['13-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: '13.32%', left: '40%'}}>
				{ props.board['12-6'] && props.board['12-6'].length ? <Goti length={props.board['12-6'].length} move={props.move} setChoice={props.setChoice} place="12-6" glow={props.board['12-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['12-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: '19.98%', left: '40%'}}>
				{ props.board['11-6'] && props.board['11-6'].length ? <Goti length={props.board['11-6'].length} move={props.move} setChoice={props.setChoice} place="11-6" glow={props.board['11-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['11-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: '26.64%', left: '40%'}}>
				{ props.board['10-6'] && props.board['10-6'].length ? <Goti length={props.board['10-6'].length} move={props.move} setChoice={props.setChoice} place="10-6" glow={props.board['10-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['10-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{bottom: '33.3%', left: '40%'}}>
				{ props.board['9-6'] && props.board['9-6'].length ? <Goti length={props.board['9-6'].length} move={props.move} setChoice={props.setChoice} place="9-6" glow={props.board['9-6'].includes(props.turn) && !props.choice} color={props.colors[props.board['9-6'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', left: '33.3%'}}>
				{ props.board['8-5'] && props.board['8-5'].length ? <Goti length={props.board['8-5'].length} move={props.move} setChoice={props.setChoice} place="8-5" glow={props.board['8-5'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-5'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', left: '26.64%'}}>
				{ props.board['8-4'] && props.board['8-4'].length ? <Goti length={props.board['8-4'].length} move={props.move} setChoice={props.setChoice} place="8-4" glow={props.board['8-4'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-4'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', left: '19.98%'}}>
				{ props.board['8-3'] && props.board['8-3'].length ? <Goti length={props.board['8-3'].length} move={props.move} setChoice={props.setChoice} place="8-3" glow={props.board['8-3'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-3'][0]]} /> : null }
			</div>
			<div className="cells safe" style={{top: '53.32%', left: '13.32%'}} />
			<div className="cells" style={{top: '53.32%', left: '13.32%'}}>
				{ props.board['8-2'] && props.board['8-2'].length ? <Goti length={props.board['8-2'].length} move={props.move} setChoice={props.setChoice} place="8-2" glow={props.board['8-2'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-2'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', left: '6.66%'}}>
				{ props.board['8-1'] && props.board['8-1'].length ? <Goti length={props.board['8-1'].length} move={props.move} setChoice={props.setChoice} place="8-1" glow={props.board['8-1'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-1'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '53.32%', left: 0}}>
				{ props.board['8-0'] && props.board['8-0'].length ? <Goti length={props.board['8-0'].length} move={props.move} setChoice={props.setChoice} place="8-0" glow={props.board['8-0'].includes(props.turn) && !props.choice} color={props.colors[props.board['8-0'][0]]} /> : null }
			</div>
			<div className="cells green" style={{top: '46.66%', left: '33.3%'}}>
				{ props.board['7-5'] && props.board['7-5'].length ? <Goti length={props.board['7-5'].length} move={props.move} setChoice={props.setChoice} place="7-5" glow={props.board['7-5'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-5'][0]]} /> : null }
			</div>
			<div className="cells green" style={{top: '46.66%', left: '26.64%'}}>
				{ props.board['7-4'] && props.board['7-4'].length ? <Goti length={props.board['7-4'].length} move={props.move} setChoice={props.setChoice} place="7-4" glow={props.board['7-4'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-4'][0]]} /> : null }
			</div>
			<div className="cells green" style={{top: '46.66%', left: '19.98%'}}>
				{ props.board['7-3'] && props.board['7-3'].length ? <Goti length={props.board['7-3'].length} move={props.move} setChoice={props.setChoice} place="7-3" glow={props.board['7-3'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-3'][0]]} /> : null }
			</div>
			<div className="cells green" style={{top: '46.66%', left: '13.32%'}}>
				{ props.board['7-2'] && props.board['7-2'].length ? <Goti length={props.board['7-2'].length} move={props.move} setChoice={props.setChoice} place="7-2" glow={props.board['7-2'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-2'][0]]} /> : null }
			</div>
			<div className="cells green" style={{top: '46.66%', left: '6.66%'}}>
				{ props.board['7-1'] && props.board['7-1'].length ? <Goti length={props.board['7-1'].length} move={props.move} setChoice={props.setChoice} place="7-1" glow={props.board['7-1'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-1'][0]]} /> : null }
			</div>
			<div className="cells" style={{top: '46.66%', left: 0}}>
				{ props.board['7-0'] && props.board['7-0'].length ? <Goti length={props.board['7-0'].length} move={props.move} setChoice={props.setChoice} place="7-0" glow={props.board['7-0'].includes(props.turn) && !props.choice} color={props.colors[props.board['7-0'][0]]} /> : null }
			</div>
		</div>
    )
}