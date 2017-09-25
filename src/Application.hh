<?hh // strict
namespace Usox\Sharesta;

final class Application implements ApplicationInterface {

	const HTTP_OK = 200;
	const HTTP_BAD_REQUEST = 400;
	const HTTP_NOT_FOUND = 404;
	const HTTP_INTERNAL_SERVER_ERROR = 500;

	public function __construct(
		private ApiFactoryInterface $api_factory,
		private RequestInterface $request,
		private RouterInterface $router
	): void {
	}

	public function handle(string $base_path): void {
		try {
			$this->sendResponse(
				static::HTTP_OK,
				$this->router->route($this->request, $base_path)
			);
		} catch (Exception\NotFoundException $e) {
			$this->sendResponse(
				static::HTTP_NOT_FOUND,
				$e->getMessage()
			);
		} catch (Exception\RequestException $e) {
			$this->sendResponse(
				static::HTTP_BAD_REQUEST,
				$e->getMessage()
			);
		} catch (Exception\ServerException $e) {
			$this->sendResponse(
				static::HTTP_INTERNAL_SERVER_ERROR,
				$e->getMessage()
			);
		} catch (\Exception $e) {
			$this->sendResponse(
				static::HTTP_INTERNAL_SERVER_ERROR,
				'Internal server error'
			);
		}
	}

	private function sendResponse(int $status_code, mixed $body): void {
		$this->api_factory
			->createResponse($status_code, $body)
			->send();	
	}
}
