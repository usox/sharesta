<?hh // strict
namespace Usox\Sharesta;

use function Facebook\FBExpect\expect;
use type Facebook\HackTest\DataProvider;

class ApiFactoryTest extends \Facebook\HackTest\HackTest {

	<<DataProvider('provideFactoryMethods')>>
	public function testCreationMethodsReturnInstances<T>(
		string $method_name,
		classname<T> $expected_class_name,
		vec<mixed> $parameter
	): void {
		$api_factory = new ApiFactory();
		expect(
			\call_user_func_array(varray[$api_factory, $method_name], $parameter)
		)
		->toBeInstanceOf($expected_class_name);
	}

	public function provideFactoryMethods(): vec<mixed> {
		return vec[
			tuple('createRouter', Router::class, vec[dict[], dict[]]),
			tuple('createResponse', Response::class, vec[666, 'roedlbroem']),
		];
	}
}
