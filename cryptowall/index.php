<?php
date_default_timezone_set('UTC');
$datetime1 = new DateTime();
$datetime2 = date_create("2018-12-24 00:13:28");
$dateDiff = date_diff($datetime1, $datetime2);
?>
<head>
	<title>Fsociety Ransomware</title>
	<link rel="stylesheet" href="styles.css" type="text/css"/>
</head>
<meta http-equiv="refresh" content="1" > 
<body>
<div id="container">
	<div id="header_ransom">
		<p class="ophello">#OPdailyallowance.</p>
	<p class="ophello">Your files are encrypted.</p>
	<p class="information">	
		To get the key to decrypt files, you have to paid <span class="red">5.9 million USD.</span> if payment is not made 
		by <span class="red">tommorow night</span> we'll brick your entire system.<br /><br />
		More instructions forthcoming. - fsociety.
		<p class="time">Time left: <span class="red"><?=
		$dateDiff->format('%H'); ?></span>H <span class="red"><?=
		$dateDiff->format('%I'); ?></span>m <span class="red"><?=
		$dateDiff->format('%S'); ?></span>s </span> 
	</p>
	<div class="more_information">
		<p class="minfo" style="width:20%">Your system : <span class="red">(x64)</span></p>
		<p class="minfo" style="width:35%">First connect IP : <span class="red">192.168.1.2    
		<img src="flags.png" class="flag flag-us" alt="United States"/></span></p>
		<p class="minfo" style="width:40%">Total encrypted : <span
		class="red">0,010</span> encrypted files.</p>
	</div>
	</div>
	<div id="menu_ransom">
	<a href="#"><button class="button">Refresh</button></a>
	<button class="button red">Payment</button>
	<button class="button">FAQ</button>
	<button class="button red">Decrypt one file for free</button>
	<button class="button">Support</button>
	</div>
	<div id="jester"></div>
	<div class="diclaimer">
	JΞSTΞR ✪ ΔCTUAL³³º¹ <br /> <br />
	There's an unequal amount of good and bad in most things. The trick is to
	figure out the ratio and act accordingly.
	</div>
</div>
</div>
