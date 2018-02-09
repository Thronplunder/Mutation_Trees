(
~branches1 = ();
~branches2 = ();
~branches3 = ();
~branches4 = ();
~branches5 = ();
~branches6 = ();
~branches7 = ();
~branches8 = ();
)
(
SynthDef(\branch, {|freq = 440, amp = 0.03, gate = 1, pan = 0, channel = 0|
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
	//msg[1].postln;
	switch(screen,
		1, {channel = [0,1];
			if(~branches1[id] == nil, {~branches1[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
		},
		2, {channel = [0,1];
			if(~branches2[id] == nil, {~branches2[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
		},
		3, {channel = [0,1];
			if(~branches3[id] == nil, {~branches3[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
		},
		4, {channel = [0,1];
			if(~branches4[id] == nil, {~branches4[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
		},
		5, {channel = [0,1];
			if(~branches5[id] == nil, {~branches5[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
		},
		6, {channel = [0,1];
			if(~branches6[id] == nil, {~branches6[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
		},
		7, {channel = [0,1];
			if(~branches7[id] == nil, {~branches7[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
		},
		8, {channel = [0,1];
			if(~branches8[id] == nil, {~branches8[id] = Synth(\branch, [\freq, (Scale.major.degrees.choose+48 + (12 * (4-msg[2]))).midicps, \gate, 1, \pan, rrand(-1.0, 1.0), \channel, channel]);},
		{
			"ID already taken".postln;
	});
	});

}, "/born",recvPort: 57120 );

OSCdef(\death, {|msg, time, addr, recvPort|
	var id = msg[1].asSymbol;
	var screen = msg[3].asInteger;
	switch(screen,1,{
		~branches1[id].set(\gate, 1);
		~branches1[id].set(\gate, 0);
		~branches1[id] = nil;
	},
		2,{
		~branches2[id].set(\gate, 1);
		~branches2[id].set(\gate, 0);
		~branches2[id] = nil;},
		3,{
		~branches3[id].set(\gate, 1);
		~branches3[id].set(\gate, 0);
		~branches3[id] = nil;
		},
		4,{
		~branches4[id].set(\gate, 1);
		~branches4[id].set(\gate, 0);
		~branches4[id] = nil;
	},
		5,{
		~branches5[id].set(\gate, 1);
		~branches5[id].set(\gate, 0);
		~branches5[id] = nil;
	},
		6,{
		~branches6[id].set(\gate, 1);
		~branches6[id].set(\gate, 0);
		~branches6[id] = nil;
	},
		7,{
		~branches7[id].set(\gate, 1);
		~branches7[id].set(\gate, 0);
		~branches7[id] = nil;
	},
		8,{
		~branches8[id].set(\gate, 1);
		~branches8[id].set(\gate, 0);
		~branches8[id] = nil;
	});

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