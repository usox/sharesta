<?hh // strict
namespace Usox\Sharesta;

interface ApiFactoryInterface {

	public function createRouter(): RouterInterface;

	public function createRequest(
		string $path,
		Map<string,string> $get_vars,
		Map<string,string> $server_vars
	): RequestInterface;

	public function createApplication(
		RequestInterface $request,
		RouterInterface $router
	): ApplicationInterface;

	public function createResponse(int $status_code, mixed $body): ResponseInterface;
}
