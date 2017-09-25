<?hh // decl
namespace Usox\Sharesta;

use Mockery as m;

class ApplicationTest extends \PHPUnit_Framework_TestCase {

	private RequestInterface $request;

	private ApiFactoryInterface $api_factory;

	private RouterInterface $router;

	private string $base_path = 'a-nice-path';

	private Application $application;

	public function setUp() {
		$this->api_factory = m::mock(ApiFactoryInterface::class);
		$this->router = m::mock(RouterInterface::class);

		$this->application = new Application(
			$this->api_factory,
			$this->router
		);
	}

	/**
	 * @dataProvider handleDataProvider
	 */
	public function testHandleCreatesCorrectResponse(
		int $status_code,
		string $exception_class,
		string $error_message
	): void {
		$exception = new $exception_class($error_message);

		$this->router
			->shouldReceive('route')
			->with($this->base_path)
			->once()
			->andThrow($exception);

		$this->setExpectedResponse($status_code, $error_message);

		$this->assertNull(
			$this->application->handle($this->base_path)
		);
	}

	public function handleDataProvider() {
		$error_message = 'a-fancy-error-message';

		return [
			[Application::HTTP_NOT_FOUND , Exception\NotFoundException::class, $error_message],
			[Application::HTTP_BAD_REQUEST, Exception\RequestException::class, $error_message],
			[Application::HTTP_INTERNAL_SERVER_ERROR, Exception\ServerException::class, $error_message],
			[Application::HTTP_INTERNAL_SERVER_ERROR, \Exception::class, 'Internal server error'],
		];
	}

	public function testHandleSendOkResponse() {
		$message = 'a-fancy-message';

		$this->setExpectedResponse(200, $message);

		$this->router
			->shouldReceive('route')
			->with($this->base_path)
			->once()
			->andReturn($message);

		$this->assertNull(
			$this->application->handle($this->base_path)
		);
	}

	private function setExpectedResponse($code, $message) {
		$response = m::mock(ResponseInterface::class);
		$response
			->shouldReceive('send')
			->once();

		$this->api_factory
			->shouldReceive('createResponse')
			->with($code, $message)
			->once()
			->andReturn($response);
	}
}
