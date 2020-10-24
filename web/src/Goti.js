import React from 'react';

export default function Goti(props) {
	let gotis = [];
	const size = props.block.length === 1 ? '100%' : '50%';					// size = 100% for only 1 goti, else 50% (To fill all gotis)
	for (let i = 0; i < props.block.length; ++i) {
		const glow = props.block[i] === props.turn && !props.choice;			// Glow the gotis if player is expected to make a move
		gotis.push(<figure key={i}											// Finally generate all gotis at current block for rendering
			className={'circle ' + (glow ? 'glow' : '')}
			style={{ background: 'radial-gradient(circle at 33% 33%, ' + props.colors[props.block[i]] + ', #000)', height: size, width: size }}
			{...(props.place && { onClick: () => glow ? props.move(props.place) : null })}	// Click is allowed the same time as 'glow'
		/>);
	}
	return <span>{gotis}</span>;
}
