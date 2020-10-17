import React from 'react'

export default function Goti(props) {
	let gotis = []
	const size = props.length === 1 ? "100%" : "50%"
	const style = { background: "radial-gradient(circle at 33% 33%, " + props.color + ", #000)", height: size, width: size }
	const classes = "circle " + (props.glow ? "glow" : "")
	for(let i=0;i<props.length;++i) {
		gotis.push(<figure key={i}
			className={classes}
			style={style}
			{ ...(props.place && { onClick: () => props.glow ? props.move(props.place) : null }) }
		/>)
	}
	return <span>{gotis}</span>
}
