import React from 'react';
import Turn from './Turn';

export default function Dice(props) {
	return (
		<div className="dice">
			<Turn turn={props.turns} color={props.color} choice={props.choice} />
			<ol className="die-list even-roll" data-roll="1" style={{ cursor: props.choice && props.die ? 'pointer' : null }} onClick={() => { props.rollDice(props.turn); }}>
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
	);
}
