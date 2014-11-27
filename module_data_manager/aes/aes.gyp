{
'target_defaults': {
	'conditions':
		[
			['OS == "ios"',
		      {
		        'xcode_settings': {
		          'TARGETED_DEVICE_FAMILY': '1,2',
		          'CODE_SIGN_IDENTITY': 'iPhone Developer',
		          'IPHONEOS_DEPLOYMENT_TARGET': '5.0',
		          'ARCHS': '$(ARCHS_STANDARD_32_64_BIT)',
		        }, # xcode_settings
		      },
		    ],  # OS == "ios"
		]
	},
	'targets':[
		{
			'target_name': 'libaes',
			'type': 'static_library',
			'sources':
			[
				'aes_my.h',
				'aes.h',
				'aes_my.c',
				'aes.c'
			],
		}
	],
'conditions': [
    ['OS == "ios"',
      {
        'xcode_settings': {
          'SDKROOT': 'iphoneos',
        }, # xcode_settings
      },
    ],  # OS == "ios"
    [
	'OS == "pc"',
	  {
		'targets': [
            {
				'target_name': 'aes',
				'type': 'executable',
				'dependencies': [
	              'libaes',
	            ],
				'sources':
				[
					'aes.h',
					'test/main.c',
				]
			}
        ] #targets
	  }
    ]
  ]
}