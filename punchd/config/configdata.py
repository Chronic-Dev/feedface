'all': {
    'iPhone2,1_4.2.1': {
        '<': '.armv7',
    },
    'iPhone3,1_4.2.1': {
        '<': '.armv7',
    },
    'iPod2,1_4.2.1': {
        '<': '.armv6',
    },
    'iPod4,1_4.2.1': {
        '<': '.armv7',
    },
},
'world1': {
    '.base': {},
    '.armv7': {
        '<':            '.base',
        'arch':         'armv7',
        
        'is_armv7': 1,

        '#dyld_shared_cache_armv7':{
            # pop {r0-r3, pc}
            'k7': '@ + 0f bd',
            # pop {r4-r7, pc}
            'k11': '@ + f0 bd',
            # blx r4; pop {r4, r5, r7, pc}
            'k12': '@ + a0 47 b0 bd',
            
            # pop {r6,r7} ; bx lr
            'k101' : '@ - c0 00 bd e8 1e ff 2f e1',
            # mov r0, #0; bx lr  (dyld bin is executable?)
            'k102' : '@ + 00 20 70 47',
            # SUB SP, R7, #0 ; pop {R7,PC}
            'k103' : '@ + A7 F1 00 0D 80 BD 00 BF',

        },

    },

    '.armv6': {
        '<':            '.base',
        'arch':         'armv6',
        
        'is_armv7': 0,
        '#dyld_shared_cache_armv6':{
            # pop {r0-r3, pc}
            'k7': '@ + 0f bd',
            # pop {r4-r7, pc}
            'k11': '@ + f0 bd',
            # blx r4; pop {r4, r5, r7, pc}
            'k12': '@ + a0 47 b0 bd',
            
            # pop {r6,r7} ; bx lr
            'k101' : '@ - c0 00 bd e8 1e ff 2f e1',
            # mov r0, #0; bx lr  (dyld bin is executable?)
            'k102' : '@ + 00 20 70 47',
            # SUB SP, R7, #0 ; pop {R7,PC}
            'k103' : '@ + A7 F1 00 0D 80 BD 00 BF',
        },

    },

},
