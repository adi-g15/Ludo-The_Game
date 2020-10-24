import React from 'react';
import Goti from './Goti';

export default function Board(props) {
	return (
		<div className="game">
			<div className="house green">
				<div className="box">
					<div className="square square-one green">
						{props.positions[0].reduce((total, curr) => total + (curr === false), 0) > 0 ? <Goti block={[0]} colors={props.colors} /> : null}
					</div>
					<div className="square square-two green">
						{props.positions[0].reduce((total, curr) => total + (curr === false), 0) > 1 ? <Goti block={[0]} colors={props.colors} /> : null}
					</div>
					<div className="square square-three green">
						{props.positions[0].reduce((total, curr) => total + (curr === false), 0) > 2 ? <Goti block={[0]} colors={props.colors} /> : null}
					</div>
					<div className="square square-four green">
						{props.positions[0].reduce((total, curr) => total + (curr === false), 0) > 3 ? <Goti block={[0]} colors={props.colors} /> : null}
					</div>
				</div>
			</div>
			<div className="house yellow" style={{ right: 0 }}>
				<div className="box">
					<div className="square square-one yellow">
						{props.positions[1].reduce((total, curr) => total + (curr === false), 0) > 0 ? <Goti block={[1]} colors={props.colors} /> : null}
					</div>
					<div className="square square-two yellow">
						{props.positions[1].reduce((total, curr) => total + (curr === false), 0) > 1 ? <Goti block={[1]} colors={props.colors} /> : null}
					</div>
					<div className="square square-three yellow">
						{props.positions[1].reduce((total, curr) => total + (curr === false), 0) > 2 ? <Goti block={[1]} colors={props.colors} /> : null}
					</div>
					<div className="square square-four yellow">
						{props.positions[1].reduce((total, curr) => total + (curr === false), 0) > 3 ? <Goti block={[1]} colors={props.colors} /> : null}
					</div>
				</div>
			</div>
			<div className="house red" style={{ bottom: 0 }}>
				<div className="box">
					<div className="square square-one red">
						{props.positions[3].reduce((total, curr) => total + (curr === false), 0) > 0 ? <Goti block={[3]} colors={props.colors} /> : null}
					</div>
					<div className="square square-two red">
						{props.positions[3].reduce((total, curr) => total + (curr === false), 0) > 1 ? <Goti block={[3]} colors={props.colors} /> : null}
					</div>
					<div className="square square-three red">
						{props.positions[3].reduce((total, curr) => total + (curr === false), 0) > 2 ? <Goti block={[3]} colors={props.colors} /> : null}
					</div>
					<div className="square square-four red">
						{props.positions[3].reduce((total, curr) => total + (curr === false), 0) > 3 ? <Goti block={[3]} colors={props.colors} /> : null}
					</div>
				</div>
			</div>
			<div className="house blue" style={{ bottom: 0, right: 0 }}>
				<div className="box">
					<div className="square square-one blue">
						{props.positions[2].reduce((total, curr) => total + (curr === false), 0) > 0 ? <Goti block={[2]} colors={props.colors} /> : null}
					</div>
					<div className="square square-two blue">
						{props.positions[2].reduce((total, curr) => total + (curr === false), 0) > 1 ? <Goti block={[2]} colors={props.colors} /> : null}
					</div>
					<div className="square square-three blue">
						{props.positions[2].reduce((total, curr) => total + (curr === false), 0) > 2 ? <Goti block={[2]} colors={props.colors} /> : null}
					</div>
					<div className="square square-four blue">
						{props.positions[2].reduce((total, curr) => total + (curr === false), 0) > 3 ? <Goti block={[2]} colors={props.colors} /> : null}
					</div>
				</div>
			</div>
			<div className="home" />
			<div className="cells" style={{ top: '40%' }}>
				{props.board['6-0'] && props.board['6-0'].length ? <Goti block={props.board['6-0']} move={props.move} setChoice={props.setChoice} place="6-0" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells g-start" style={{ top: '40%', left: '6.66%' }} />
			<div className="cells" style={{ top: '40%', left: '6.66%' }} >
				{props.board['6-1'] && props.board['6-1'].length ? <Goti block={props.board['6-1']} move={props.move} setChoice={props.setChoice} place="6-1" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', left: '13.32%' }}>
				{props.board['6-2'] && props.board['6-2'].length ? <Goti block={props.board['6-2']} move={props.move} setChoice={props.setChoice} place="6-2" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', left: '19.98%' }}>
				{props.board['6-3'] && props.board['6-3'].length ? <Goti block={props.board['6-3']} move={props.move} setChoice={props.setChoice} place="6-3" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', left: '26.64%' }}>
				{props.board['6-4'] && props.board['6-4'].length ? <Goti block={props.board['6-4']} move={props.move} setChoice={props.setChoice} place="6-4" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', left: '33.3%' }}>
				{props.board['6-5'] && props.board['6-5'].length ? <Goti block={props.board['6-5']} move={props.move} setChoice={props.setChoice} place="6-5" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: 0, left: '40%' }}>
				{props.board['0-6'] && props.board['0-6'].length ? <Goti block={props.board['0-6']} move={props.move} setChoice={props.setChoice} place="0-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '6.66%', left: '40%' }}>
				{props.board['1-6'] && props.board['1-6'].length ? <Goti block={props.board['1-6']} move={props.move} setChoice={props.setChoice} place="1-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells safe" style={{ top: '13.32%', left: '40%' }} />
			<div className="cells" style={{ top: '13.32%', left: '40%' }}>
				{props.board['2-6'] && props.board['2-6'].length ? <Goti block={props.board['2-6']} move={props.move} setChoice={props.setChoice} place="2-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '19.98%', left: '40%' }}>
				{props.board['3-6'] && props.board['3-6'].length ? <Goti block={props.board['3-6']} move={props.move} setChoice={props.setChoice} place="3-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '26.64%', left: '40%' }}>
				{props.board['4-6'] && props.board['4-6'].length ? <Goti block={props.board['4-6']} move={props.move} setChoice={props.setChoice} place="4-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '33.3%', left: '40%' }}>
				{props.board['5-6'] && props.board['5-6'].length ? <Goti block={props.board['5-6']} move={props.move} setChoice={props.setChoice} place="5-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: 0, left: '46.66%' }}>
				{props.board['0-7'] && props.board['0-7'].length ? <Goti block={props.board['0-7']} move={props.move} setChoice={props.setChoice} place="0-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells yellow" style={{ top: '6.66%', left: '46.66%' }}>
				{props.board['1-7'] && props.board['1-7'].length ? <Goti block={props.board['1-7']} move={props.move} setChoice={props.setChoice} place="1-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells yellow" style={{ top: '13.32%', left: '46.66%' }}>
				{props.board['2-7'] && props.board['2-7'].length ? <Goti block={props.board['2-7']} move={props.move} setChoice={props.setChoice} place="2-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells yellow" style={{ top: '19.98%', left: '46.66%' }}>
				{props.board['3-7'] && props.board['3-7'].length ? <Goti block={props.board['3-7']} move={props.move} setChoice={props.setChoice} place="3-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells yellow" style={{ top: '26.64%', left: '46.66%' }}>
				{props.board['4-7'] && props.board['4-7'].length ? <Goti block={props.board['4-7']} move={props.move} setChoice={props.setChoice} place="4-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells yellow" style={{ top: '33.3%', left: '46.66%' }}>
				{props.board['5-7'] && props.board['5-7'].length ? <Goti block={props.board['5-7']} move={props.move} setChoice={props.setChoice} place="5-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: 0, left: '53.32%' }}>
				{props.board['0-8'] && props.board['0-8'].length ? <Goti block={props.board['0-8']} move={props.move} setChoice={props.setChoice} place="0-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells y-start" style={{ top: '6.66%', left: '53.32%' }} />
			<div className="cells" style={{ top: '6.66%', left: '53.32%' }}>
				{props.board['1-8'] && props.board['1-8'].length ? <Goti block={props.board['1-8']} move={props.move} setChoice={props.setChoice} place="1-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '13.32%', left: '53.32%' }}>
				{props.board['2-8'] && props.board['2-8'].length ? <Goti block={props.board['2-8']} move={props.move} setChoice={props.setChoice} place="2-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '19.98%', left: '53.32%' }}>
				{props.board['3-8'] && props.board['3-8'].length ? <Goti block={props.board['3-8']} move={props.move} setChoice={props.setChoice} place="3-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '26.64%', left: '53.32%' }}>
				{props.board['4-8'] && props.board['4-8'].length ? <Goti block={props.board['4-8']} move={props.move} setChoice={props.setChoice} place="4-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '33.3%', left: '53.32%' }}>
				{props.board['5-8'] && props.board['5-8'].length ? <Goti block={props.board['5-8']} move={props.move} setChoice={props.setChoice} place="5-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', right: '33.3%' }}>
				{props.board['6-9'] && props.board['6-9'].length ? <Goti block={props.board['6-9']} move={props.move} setChoice={props.setChoice} place="6-9" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', right: '26.64%' }}>
				{props.board['6-10'] && props.board['6-10'].length ? <Goti block={props.board['6-10']} move={props.move} setChoice={props.setChoice} place="6-10" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', right: '19.98%' }}>
				{props.board['6-11'] && props.board['6-11'].length ? <Goti block={props.board['6-11']} move={props.move} setChoice={props.setChoice} place="6-11" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells safe" style={{ top: '40%', right: '13.32%' }} />
			<div className="cells" style={{ top: '40%', right: '13.32%' }}>
				{props.board['6-12'] && props.board['6-12'].length ? <Goti block={props.board['6-12']} move={props.move} setChoice={props.setChoice} place="6-12" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', right: '6.66%' }}>
				{props.board['6-13'] && props.board['6-13'].length ? <Goti block={props.board['6-13']} move={props.move} setChoice={props.setChoice} place="6-13" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '40%', right: 0 }}>
				{props.board['6-14'] && props.board['6-14'].length ? <Goti block={props.board['6-14']} move={props.move} setChoice={props.setChoice} place="6-14" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells blue" style={{ top: '46.66%', right: '33.3%' }}>
				{props.board['7-9'] && props.board['7-9'].length ? <Goti block={props.board['7-9']} move={props.move} setChoice={props.setChoice} place="7-9" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells blue" style={{ top: '46.66%', right: '26.64%' }}>
				{props.board['7-10'] && props.board['7-10'].length ? <Goti block={props.board['7-10']} move={props.move} setChoice={props.setChoice} place="7-10" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells blue" style={{ top: '46.66%', right: '19.98%' }}>
				{props.board['7-11'] && props.board['7-11'].length ? <Goti block={props.board['7-11']} move={props.move} setChoice={props.setChoice} place="7-11" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells blue" style={{ top: '46.66%', right: '13.32%' }}>
				{props.board['7-12'] && props.board['7-12'].length ? <Goti block={props.board['7-12']} move={props.move} setChoice={props.setChoice} place="7-12" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells blue" style={{ top: '46.66%', right: '6.66%' }}>
				{props.board['7-13'] && props.board['7-13'].length ? <Goti block={props.board['7-13']} move={props.move} setChoice={props.setChoice} place="7-13" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '46.66%', right: 0 }}>
				{props.board['7-14'] && props.board['7-14'].length ? <Goti block={props.board['7-14']} move={props.move} setChoice={props.setChoice} place="7-14" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', right: '33.3%' }}>
				{props.board['8-9'] && props.board['8-9'].length ? <Goti block={props.board['8-9']} move={props.move} setChoice={props.setChoice} place="8-9" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', right: '26.64%' }}>
				{props.board['8-10'] && props.board['8-10'].length ? <Goti block={props.board['8-10']} move={props.move} setChoice={props.setChoice} place="8-10" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', right: '19.98%' }}>
				{props.board['8-11'] && props.board['8-11'].length ? <Goti block={props.board['8-11']} move={props.move} setChoice={props.setChoice} place="8-11" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', right: '13.32%' }}>
				{props.board['8-12'] && props.board['8-12'].length ? <Goti block={props.board['8-12']} move={props.move} setChoice={props.setChoice} place="8-12" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells b-start" style={{ top: '53.32%', right: '6.66%' }} />
			<div className="cells" style={{ top: '53.32%', right: '6.66%' }}>
				{props.board['8-13'] && props.board['8-13'].length ? <Goti block={props.board['8-13']} move={props.move} setChoice={props.setChoice} place="8-13" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', right: 0 }}>
				{props.board['8-14'] && props.board['8-14'].length ? <Goti block={props.board['8-14']} move={props.move} setChoice={props.setChoice} place="8-14" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: 0, left: '53.32%' }}>
				{props.board['14-8'] && props.board['14-8'].length ? <Goti block={props.board['14-8']} move={props.move} setChoice={props.setChoice} place="14-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: '6.66%', left: '53.32%' }}>
				{props.board['13-8'] && props.board['13-8'].length ? <Goti block={props.board['13-8']} move={props.move} setChoice={props.setChoice} place="13-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells safe" style={{ bottom: '13.32%', left: '53.32%' }} />
			<div className="cells" style={{ bottom: '13.32%', left: '53.32%' }}>
				{props.board['12-8'] && props.board['12-8'].length ? <Goti block={props.board['12-8']} move={props.move} setChoice={props.setChoice} place="12-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: '19.98%', left: '53.32%' }}>
				{props.board['11-8'] && props.board['11-8'].length ? <Goti block={props.board['11-8']} move={props.move} setChoice={props.setChoice} place="11-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: '26.64%', left: '53.32%' }}>
				{props.board['10-8'] && props.board['10-8'].length ? <Goti block={props.board['10-8']} move={props.move} setChoice={props.setChoice} place="10-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: '33.3%', left: '53.32%' }}>
				{props.board['9-8'] && props.board['9-8'].length ? <Goti block={props.board['9-8']} move={props.move} setChoice={props.setChoice} place="9-8" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: 0, left: '46.66%' }}>
				{props.board['14-7'] && props.board['14-7'].length ? <Goti block={props.board['14-7']} move={props.move} setChoice={props.setChoice} place="14-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells red " style={{ bottom: '6.66%', left: '46.66%' }}>
				{props.board['13-7'] && props.board['13-7'].length ? <Goti block={props.board['13-7']} move={props.move} setChoice={props.setChoice} place="13-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells red" style={{ bottom: '13.32%', left: '46.66%' }}>
				{props.board['12-7'] && props.board['12-7'].length ? <Goti block={props.board['12-7']} move={props.move} setChoice={props.setChoice} place="12-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells red" style={{ bottom: '19.98%', left: '46.66%' }}>
				{props.board['11-7'] && props.board['11-7'].length ? <Goti block={props.board['11-7']} move={props.move} setChoice={props.setChoice} place="11-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells red" style={{ bottom: '26.64%', left: '46.66%' }}>
				{props.board['10-7'] && props.board['10-7'].length ? <Goti block={props.board['10-7']} move={props.move} setChoice={props.setChoice} place="10-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells red" style={{ bottom: '33.3%', left: '46.66%' }}>
				{props.board['9-7'] && props.board['9-7'].length ? <Goti block={props.board['9-7']} move={props.move} setChoice={props.setChoice} place="9-7" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: 0, left: '40%' }}>
				{props.board['14-6'] && props.board['14-6'].length ? <Goti block={props.board['14-6']} move={props.move} setChoice={props.setChoice} place="14-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells r-start" style={{ bottom: '6.66%', left: '40%' }} />
			<div className="cells" style={{ bottom: '6.66%', left: '40%' }}>
				{props.board['13-6'] && props.board['13-6'].length ? <Goti block={props.board['13-6']} move={props.move} setChoice={props.setChoice} place="13-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: '13.32%', left: '40%' }}>
				{props.board['12-6'] && props.board['12-6'].length ? <Goti block={props.board['12-6']} move={props.move} setChoice={props.setChoice} place="12-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: '19.98%', left: '40%' }}>
				{props.board['11-6'] && props.board['11-6'].length ? <Goti block={props.board['11-6']} move={props.move} setChoice={props.setChoice} place="11-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: '26.64%', left: '40%' }}>
				{props.board['10-6'] && props.board['10-6'].length ? <Goti block={props.board['10-6']} move={props.move} setChoice={props.setChoice} place="10-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ bottom: '33.3%', left: '40%' }}>
				{props.board['9-6'] && props.board['9-6'].length ? <Goti block={props.board['9-6']} move={props.move} setChoice={props.setChoice} place="9-6" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', left: '33.3%' }}>
				{props.board['8-5'] && props.board['8-5'].length ? <Goti block={props.board['8-5']} move={props.move} setChoice={props.setChoice} place="8-5" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', left: '26.64%' }}>
				{props.board['8-4'] && props.board['8-4'].length ? <Goti block={props.board['8-4']} move={props.move} setChoice={props.setChoice} place="8-4" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', left: '19.98%' }}>
				{props.board['8-3'] && props.board['8-3'].length ? <Goti block={props.board['8-3']} move={props.move} setChoice={props.setChoice} place="8-3" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells safe" style={{ top: '53.32%', left: '13.32%' }} />
			<div className="cells" style={{ top: '53.32%', left: '13.32%' }}>
				{props.board['8-2'] && props.board['8-2'].length ? <Goti block={props.board['8-2']} move={props.move} setChoice={props.setChoice} place="8-2" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', left: '6.66%' }}>
				{props.board['8-1'] && props.board['8-1'].length ? <Goti block={props.board['8-1']} move={props.move} setChoice={props.setChoice} place="8-1" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '53.32%', left: 0 }}>
				{props.board['8-0'] && props.board['8-0'].length ? <Goti block={props.board['8-0']} move={props.move} setChoice={props.setChoice} place="8-0" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells green" style={{ top: '46.66%', left: '33.3%' }}>
				{props.board['7-5'] && props.board['7-5'].length ? <Goti block={props.board['7-5']} move={props.move} setChoice={props.setChoice} place="7-5" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells green" style={{ top: '46.66%', left: '26.64%' }}>
				{props.board['7-4'] && props.board['7-4'].length ? <Goti block={props.board['7-4']} move={props.move} setChoice={props.setChoice} place="7-4" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells green" style={{ top: '46.66%', left: '19.98%' }}>
				{props.board['7-3'] && props.board['7-3'].length ? <Goti block={props.board['7-3']} move={props.move} setChoice={props.setChoice} place="7-3" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells green" style={{ top: '46.66%', left: '13.32%' }}>
				{props.board['7-2'] && props.board['7-2'].length ? <Goti block={props.board['7-2']} move={props.move} setChoice={props.setChoice} place="7-2" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells green" style={{ top: '46.66%', left: '6.66%' }}>
				{props.board['7-1'] && props.board['7-1'].length ? <Goti block={props.board['7-1']} move={props.move} setChoice={props.setChoice} place="7-1" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
			<div className="cells" style={{ top: '46.66%', left: 0 }}>
				{props.board['7-0'] && props.board['7-0'].length ? <Goti block={props.board['7-0']} move={props.move} setChoice={props.setChoice} place="7-0" turn={props.turn} choice={props.choice} colors={props.colors} /> : null}
			</div>
		</div>
	);
}