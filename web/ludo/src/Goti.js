import React from 'react'

export default function Goti(props) {
	if(props.place) {
		const coords = props.place.split("-")
		return <figure
			className={"circle " + (props.glow ? "glow" : null)}
			style={{ background: "radial-gradient(circle at 33% 33%, " + props.color + ", #000)" }}
			onClick={() => {
				if(props.glow) {
					props.move(parseInt(coords[0]), parseInt(coords[1]))
					props.setChoice(true)
				}
			}}
		/>
	}
	return <figure 
		className={"circle " + (props.glow ? "glow" : null)}
		style={{ background: "radial-gradient(circle at 33% 33%, " + props.color + ", #000)" }}
	/>
}