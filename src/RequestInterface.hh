<?hh // strict
namespace Usox\Sharesta;

interface RequestInterface {

	public function getRoute(string $base_path): string;

	public function getHttpMethod(): string;

	public function getRouteParameters(): ImmMap<string,string>;

	public function setRouteParameters(ImmMap<string, string> $route_parameters): void;

	public function getUriValues(): ImmMap<string,string>;

	public function getBodyAsJson(): string;

	public function getBody(): Map<string,mixed>;
}
