<?hh // partial
namespace Usox\Sharesta;

use Mockery as m;

class ApiFactoryTest extends \PHPUnit_Framework_TestCase {

	public function setUp(): void {
		$this->api_factory = new ApiFactory();
	}

	/**
	 * @dataProvider factoryMethodsDataProvider
	 */
	public function testCreationMethodsReturnInstances(
		string $method_name,
		string $expected_class_name,
		array<mixed> $parameter
	) {
		$this->assertInstanceOf(
			$expected_class_name,
			call_user_func_array([$this->api_factory, $method_name], $parameter)
		);
	}

	public function factoryMethodsDataProvider(): array<mixed> {
		return [
			['createRouter', Router::class, []],
			['createRequest', Request::class, [m::mock(RouterInterface::class), Map{}, Map{}]],
			['createApplication', Application::class, [m::mock(RequestInterface::class), m::mock(RouterInterface::class)]],
			['createResponse', Response::class, [666, 'roedlbroem']],
		];
	}
}
