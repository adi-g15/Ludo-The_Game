import React from 'react';

export default function Turn(props) {
	return (
		<h1>
			<span style={{ color: props.color }}>{props.turn}:<br />{props.choice ? 'Roll the die' : 'Choose a Goti to move'}</span>
		</h1>
	);
}
