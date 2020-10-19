import React from 'react'

export default function Goti(props) {
	let gotis = []
	const size = props.block.length === 1 ? "100%" : "50%"
	for(let i=0;i<props.block.length;++i) {
		const glow = props.block[i] === props.turn && !props.choice
		gotis.push(<figure key={i}
			className={"circle " + (glow ? "glow" : "")}
			style={{ background: "radial-gradient(circle at 33% 33%, " + props.colors[props.block[i]] + ", #000)", height: size, width: size }}
			{ ...(props.place && { onClick: () => glow ? props.move(props.place) : null }) }
		/>)
	}
	return <span>{gotis}</span>
}
