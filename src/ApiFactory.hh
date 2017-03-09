<?hh // strict
namespace Usox\Sharesta;

final class ApiFactory implements ApiFactoryInterface {

	public function createRouter(): RouterInterface {
		return new Router();
	}

	public function createRequest(
		string $path,
		Map<string,string> $get_vars,
		Map<string,string> $server_vars
	): RequestInterface {
		return new Request(
			$path,
			$get_vars,
			$server_vars,
			new RequestBody()
		);
	}

	public function createApplication(
		RequestInterface $request,
		RouterInterface $router
	): ApplicationInterface {
		return new Application(
			$this,
			$request,
			$router
		);
	}

	public function createResponse(int $status_code, mixed $body): ResponseInterface {
		return new Response($status_code, $body);
	}
}
