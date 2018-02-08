s.boot
~branches = ();
(
SynthDef(\branch, {|freq = 440, amp = 0.01, gate = 1, pan = 0, channel = 0|
	var snd =  SinOsc.ar(freq);
	//var snd2 = SinOsc.ar(freq) * EnvGen.ar(Env.perc(1.5)) * amp;
	var env = EnvGen.kr(Env.adsr(1, 0.3, 0.6, 1),gate, doneAction:2);
	snd = snd * env * amp;
	DetectSilence.ar(snd + Impulse.ar(0), doneAction: Done.freeSelf);
	Out.ar(channel, Pan2.ar(snd, pan));
}).add;

OSCdef(\born, {|msg, time, addr, recvPort|
	var id = msg[1].asSymbol;
	var screen = msg[3].asInteger;
	var channel = nil;
	msg[1].postln;
	switch(screen,
		1, {channel = [0,1];},
		2, {channel = [0,1];},
		3, {channel = [0,1];},
		4, {channel = [0,1];},
		5, {channel = [0,1];},
		6, {channel = [0,1];},
		7, {channel = [0,1];},
		8, {channel = [0,1];});
	if(~branches[id] == nil, {~branches[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
}, "/born",recvPort: 57120 );

OSCdef(\death, {|msg, time, addr, recvPort|
	var id = msg[1].asSymbol;
	~branches[id].set(\gate, 1);
	~branches[id].set(\gate, 0);
	~branches[id] = nil;
}, "/die",recvPort: 57120 );
)



~branches

m = NetAddr("127.0.0.1", 57120)
x = Synth(\branch, [\freq, (Scale.major.degrees.choose + 60).midicps / 2 +1])
x.release

fork{
5.do({
3.do({|i|
m.sendMsg("/born", i, 1);
	0.001.wait;
});
3.do({|i|
m.sendMsg("/die", i, 1);
});
		1.5.wait;
	});
}

s.queryAllNodes
s.freeAll