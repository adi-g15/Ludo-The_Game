const express = require('express');
const morgan = require('morgan');
const helmet = require('helmet');
const { random } = require('underscore');

//  deepcode ignore UseCsurfForExpress: This won't be the actual API in deployment, so not needed to increase the dependency
const app = express();

// routes
const moveRouter = require('./routes/move');

const PORT = process.env.PORT || 3000;

app.use(helmet());
app.use(express.urlencoded({ extended: false }));
app.use(express.json());
app.use(morgan('dev'));

app.get('/roll', (req, res) => {
	let n = random(1, 6);
	const arr = [];
	arr.push(n);
	while (n === 6) {
		n = random(1, 6);
		arr.push(n);
	}
	// @todo - Remove 3 consecutive sixes if any

	res.json({
		roll: arr
	});
});

app.use('/move', moveRouter);

app.listen(PORT, () => {
	console.log(`Server listening on ${PORT}`);
});
