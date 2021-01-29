import React from 'react';

export default function Table(props) {
	return (
		<table style={{ margin: 'auto' }}>
			<thead>
				<tr>
					<th>Player</th>
					<th>Gotis Left</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>Green</td>
					<td>{props.positions[0].reduce((total, curr) => total - (curr === true), 4)}</td>
				</tr>
				<tr>
					<td>Yellow</td>
					<td>{props.positions[1].reduce((total, curr) => total - (curr === true), 4)}</td>
				</tr>
				<tr>
					<td>Blue</td>
					<td>{props.positions[2].reduce((total, curr) => total - (curr === true), 4)}</td>
				</tr>
				<tr>
					<td>Red</td>
					<td>{props.positions[3].reduce((total, curr) => total - (curr === true), 4)}</td>
				</tr>
			</tbody>
		</table>
	);
}
