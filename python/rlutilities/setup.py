import setuptools


setuptools.setup(
    name='RLUtilities',
    version='1.0.2',
    author='Sam Mish',
    author_email='samuelpmish@gmail.com',
    description='tools for controlling Rocket League bots',
    url='https://github.com/samuelpmish/rlutilities',
    packages=['rlutilities'],
    package_data={
        'rlutilities': [
            '*.pyd',
            '*.so',
            '__init__.py',
            '*.pyi',
        ]
    }
)
