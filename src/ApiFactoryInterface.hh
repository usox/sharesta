<?hh // strict
namespace Usox\Sharesta;

interface ApiFactoryInterface {

	public function createRouter(
		ImmMap<string,string> $get_vars,
		ImmMap<string,string> $server_vars
	): RouterInterface;

	public function createApplication(
		RouterInterface $router
	): ApplicationInterface;

	public function createResponse(int $status_code, mixed $body): ResponseInterface;
}
