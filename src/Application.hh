<?hh // strict
namespace Usox\Sharesta;

final class Application implements ApplicationInterface {

	public function __construct(
		private ApiFactoryInterface $api_factory,
		private RequestInterface $request,
		private RouterInterface $router
	): void {
	}

	public function handle(string $base_path): void {
		try {
			$this->sendResponse(
				200,
				$this->router->route($this->request, $base_path)
			);
		} catch (Exception\NotFoundException $e) {
			$this->sendResponse(404, $e->getMessage());
		} catch (Exception\RequestException $e) {
			$this->sendResponse(400, $e->getMessage());
		} catch (Exception\ServerException $e) {
			$this->sendResponse(500, $e->getMessage());
		} catch (\Exception $e) {
			$this->sendResponse(500, 'Internal server error');
		}
	}

	private function sendResponse(int $status_code, mixed $body): void {
		$this->api_factory
			->createResponse($status_code, $body)
			->send();	
	}
}
